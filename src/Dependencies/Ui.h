#pragma once
#include <Cool/Dependencies/AnySharedVariable.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include "CommandCore/CommandExecutor.h"
#include "Commands/Command_FinishedEditingVariable.h"
#include "Commands/Command_SetVariable.h"
#include "Commands/Command_SetVariableMetadata.h"

namespace Lab {

class Ui_Ref {
public:
    explicit Ui_Ref(CommandExecutor const& command_executor)
        : _command_executor{command_executor}
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
    void widget(Cool::SharedVariable<T>& var) const
    {
        ImGui::PushID(&var);
        auto const prev_value    = var.value();
        auto const prev_metadata = var.variable().metadata();
        var.variable().imgui({
            .on_value_changed =
                [&]() {
                    auto const new_value = var.value();
                    var.value()          = prev_value; // To make sure the reversible command that will be created sees the correct previous value.
                    _command_executor.execute(Command_SetVariable<T>{
                        .var_ref = var.get_ref(),
                        .value   = new_value,
                    });
                },

            .on_metadata_changed =
                [&]() {
                    auto const new_metadata   = var.variable().metadata();
                    var.variable().metadata() = prev_metadata; // To make sure the reversible command that will be created sees the correct previous value.
                    _command_executor.execute(Command_SetVariableMetadata<T>{
                                                  .var_ref  = var.get_ref(),
                                                  .metadata = new_metadata,
                                              },
                                              false /* store_in_history */);
                },

            .on_value_editing_finished =
                [&]() { _command_executor.execute(Command_FinishedEditingVariable{}); },
        });
        if (var.description())
            Cool::ImGuiExtras::help_marker(var.description()->c_str());
        ImGui::PopID();
    }

    void widget(Cool::AnySharedVariable& var) const
    {
        std::visit([&](auto&& var) { widget(var); }, var);
    }

    auto command_executor() -> auto& { return _command_executor; }

private:
    CommandExecutor _command_executor;
};

} // namespace Lab