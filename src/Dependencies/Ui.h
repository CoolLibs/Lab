#pragma once
#include <Cool/Dependencies/Input.h>
#include <Cool/Dependencies/InputFactory_Ref.h>
#include <Cool/Dependencies/VariableRegistries.h>
#include <Cool/File/File.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/NfdFileFilter/NfdFileFilter.h>
#include "CommandCore/CommandExecutor_TopLevel_Ref.h"
#include "Commands/Command_FinishedEditingVariable.h"
#include "Commands/Command_SetVariable.h"
#include "Commands/Command_SetVariableMetadata.h"

namespace Lab {

class Ui_Ref {
public:
    Ui_Ref(
        Cool::VariableRegistries&    registries,
        CommandExecutor_TopLevel_Ref command_executor,
        Cool::SetDirty_Ref           set_dirty,
        Cool::InputFactory_Ref       input_factory
    )
        : _variable_registries{registries}
        , _command_executor{command_executor}
        , _set_dirty{set_dirty}
        , _input_factory{input_factory}
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
        // TODO(JF) add a way to change the current_variable
        // And show the UI of the current_variable if it is set

        // NB: We don't lock the registry here because it is already locked above (might be clunky though)
        auto* default_variable = _variable_registries.get().of<Cool::Variable<T>>().get_mutable_ref(input._default_variable_id.raw());
        if (default_variable)
        {
            widget<T>(input._default_variable_id.raw(), *default_variable);

            if (input._description)
            {
                ImGui::SameLine();
                Cool::ImGuiExtras::help_marker(input._description->c_str());
            }
        }
        ImGui::PopID();
    }

    void widget(Cool::AnyInput& input)
    {
        std::visit([&](auto&& input) { widget(input); }, input);
    }

    void set_dirty(Cool::DirtyFlag const& flag) { _set_dirty(flag); }
    auto dirty_setter() const -> Cool::SetDirty_Ref { return _set_dirty; }

    auto variable_registries() const -> const Cool::VariableRegistries& { return _variable_registries; }
    auto variable_registries() -> Cool::VariableRegistries& { return _variable_registries; }

    auto input_factory() const -> auto { return _input_factory; }

private:
    template<typename T>
    void widget(const Cool::VariableId<T>& id, Cool::Variable<T>& variable)
    {
        const auto prev_value    = variable.value();
        const auto prev_metadata = variable.metadata();
        Cool::imgui(
            variable,
            {
                .on_value_changed =
                    [&]() {
                        const auto new_value = variable.value();
                        variable.value()       = prev_value; // To make sure the reversible command that will be created sees the correct previous value.
                        _command_executor.execute(
                                Command_SetVariable<T>{.id = id, .value = new_value}
                            ); },

                .on_metadata_changed =
                    [&]() {
                        const auto new_metadata = variable.metadata();
                        variable.metadata()       = prev_metadata; // To make sure the reversible command that will be created sees the correct previous value.
                        _command_executor.execute(
                                Command_SetVariableMetadata<T>{.id = id, .metadata = new_metadata}
                            ); },

                .on_value_editing_finished =
                    [&]() { _command_executor.execute(
                                Command_FinishedEditingVariable{}
                            ); },
            }
        );
    }

private:
    std::reference_wrapper<Cool::VariableRegistries> _variable_registries;
    CommandExecutor_TopLevel_Ref                     _command_executor;
    Cool::SetDirty_Ref                               _set_dirty;
    Cool::InputFactory_Ref                           _input_factory;
};

} // namespace Lab