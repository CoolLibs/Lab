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
        ImGui::Begin(params.name.data());
        ui_function();
        ImGui::End();
    }

    template<typename T>
    void widget(const Cool::Input<T>& input)
    {
        ImGui::PushID(&input);
        // TODO(JF) add a way to change the current_variable
        // And show the UI of the current_variable if it is set
        const auto variable = _variable_registries.get().get(input._default_variable_id);
        if (variable)
        {
            widget<T>(input._default_variable_id, *variable);
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

    void widget(Cool::Input_File& input)
    {
        Cool::ImGuiExtras::bring_attention_if(
            input.should_highlight(),
            [&] {
                input.update(_set_dirty); // TODO(JF) shouldn't be in the Ui, should be called all the frames even if we don't render the Ui
                std::filesystem::path path = input.file_watcher.path();
                if (Cool::ImGuiExtras::file_and_folder("Path", &path, Cool::NfdFileFilter::FragmentShader)) // TODO(JF) Don't hardcode the file filters
                {
                    input.file_watcher.set_path(path);
                    _set_dirty(input._dirty_flag);
                }
            }
        );
    }

    void set_dirty(Cool::DirtyFlag const& flag) { _set_dirty(flag); }
    auto dirty_setter() const -> Cool::SetDirty_Ref { return _set_dirty; }

    auto variable_registries() const -> const Cool::VariableRegistries& { return _variable_registries; }
    auto variable_registries() -> Cool::VariableRegistries& { return _variable_registries; }

    auto input_factory() const -> auto { return _input_factory; }

private:
    template<typename T>
    void widget(const Cool::VariableId<T>& id, Cool::Variable<T> variable)
    {
        Cool::imgui(
            variable,
            {
                .on_value_changed =
                    [&]() { _command_executor.execute(
                                Command_SetVariable<T>{.id = id, .value = variable.value}
                            ); },

                .on_metadata_changed =
                    [&]() { _command_executor.execute(
                                Command_SetVariableMetadata<T>{.id = id, .metadata = variable.metadata}
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