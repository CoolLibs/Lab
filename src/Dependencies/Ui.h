#pragma once
#include <Cool/Dependencies/AnySharedVariable.h>
#include <Cool/File/File.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/NfdFileFilter/NfdFileFilter.h>
#include "CommandCore/CommandExecutor.h"
#include "Commands/Command_FinishedEditingVariable.h"
#include "Commands/Command_SetVariable.h"
#include "Commands/Command_SetVariableMetadata.h"
#include "Cool/Audio/AudioManager.h"

namespace Lab {

class Ui_Ref {
public:
    Ui_Ref(
        CommandExecutor     command_executor,
        Cool::AudioManager& audio_manager
    )
        : _command_executor{command_executor}
        , _audio_manager{audio_manager}
    {
    }

    struct WindowParams {
        std::string_view name;
    };

    template<typename UiFunction>
    static void window(const WindowParams& params, UiFunction&& ui_function)
    {
        ImGui::Begin(params.name.data(), nullptr, ImGuiWindowFlags_NoFocusOnAppearing);
        ui_function();
        ImGui::End();
    }

    template<typename T>
    void widget(Cool::SharedVariable<T>& var)
    {
        ImGui::PushID(&var);
        auto const prev_value    = var.value();
        auto const prev_metadata = var.variable().metadata();
        var.variable().imgui(
            {
                .on_value_changed =
                    [&]() {
                        const auto new_value =  var.value();
                         var.value()       = prev_value; // To make sure the reversible command that will be created sees the correct previous value.
                        _command_executor.execute(
                                Command_SetVariable<T>{.var_ref = var.get_ref(), .value = new_value,}
                            ); },

                .on_metadata_changed =
                    [&]() {
                        auto const new_metadata   = var.variable().metadata();
                        var.variable().metadata() = prev_metadata; // To make sure the reversible command that will be created sees the correct previous value.
                        _command_executor.execute(
                            Command_SetVariableMetadata<T>{.var_ref = var.get_ref(), .metadata = new_metadata}
                        );
                    },

                .on_value_editing_finished =
                    [&]() { _command_executor.execute(
                                Command_FinishedEditingVariable{}
                            ); },
            }
        );
        if (var.description())
            Cool::ImGuiExtras::help_marker(var.description()->c_str());
        ImGui::PopID();
    }

    void widget(Cool::AnySharedVariable& var)
    {
        std::visit([&](auto&& var) { widget(var); }, var);
    }

    auto command_executor() -> auto& { return _command_executor; }
    auto audio_manager() -> Cool::AudioManager& { return _audio_manager; }

private:
    CommandExecutor     _command_executor;
    Cool::AudioManager& _audio_manager;
};

} // namespace Lab