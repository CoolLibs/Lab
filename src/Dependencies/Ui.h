#pragma once
#include <Cool/Dependencies/AnyInput.h>
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
    void widget(Cool::Input<T>& input)
    {
        ImGui::PushID(&input);
        auto const prev_value    = input.value();
        auto const prev_metadata = input._variable->metadata();
        input._variable->imgui(
            {
                .on_value_changed =
                    [&]() {
                        const auto new_value =  input.value();
                         input.value()       = prev_value; // To make sure the reversible command that will be created sees the correct previous value.
                        _command_executor.execute(
                                Command_SetVariable<T>{.input =  input, .value = new_value,}
                            ); },

                .on_metadata_changed =
                    [&]() {
                        auto const new_metadata     = input._variable->metadata();
                        input._variable->metadata() = prev_metadata; // To make sure the reversible command that will be created sees the correct previous value.
                        _command_executor.execute(
                            Command_SetVariableMetadata<T>{.input = input, .metadata = new_metadata}
                        );
                    },

                .on_value_editing_finished =
                    [&]() { _command_executor.execute(
                                Command_FinishedEditingVariable{}
                            ); },
            }
        );
        if (input._description)
            Cool::ImGuiExtras::help_marker(input._description->c_str());
        ImGui::PopID();
    }

    void widget(Cool::AnyInput& input)
    {
        std::visit([&](auto&& input) { widget(input); }, input);
    }

    auto command_executor() -> auto& { return _command_executor; }
    auto audio_manager() -> Cool::AudioManager& { return _audio_manager; }

private:
    CommandExecutor     _command_executor;
    Cool::AudioManager& _audio_manager;
};

} // namespace Lab