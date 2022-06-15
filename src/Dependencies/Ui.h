#pragma once
#include <Cool/File/File.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/NfdFileFilter/NfdFileFilter.h>
#include "CommandCore/CommandExecutor_TopLevel_Ref.h"
#include "Commands/Command_FinishedEditingValue.h"
#include "Commands/Command_SetValue.h"
#include "Input.h"
#include "VariableRegistries.h"

namespace Lab {

class Ui {
public:
    Ui(VariableRegistries&          registries,
       CommandExecutor_TopLevel_Ref command_executor,
       SetDirtyFlag_Ref             set_dirty)
        : _variable_registries{registries}
        , _command_executor{command_executor}
        , _set_dirty{set_dirty}
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

    void widget(std::string_view name, reg::Id<glm::vec3> color_id, glm::vec3 current_value)
    {
        if (ImGui::ColorEdit3(name.data(), glm::value_ptr(current_value), ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float)) {
            _command_executor.execute(
                Command_SetValue<glm::vec3>{.id    = color_id,
                                            .value = current_value});
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            _command_executor.execute(
                Command_FinishedEditingValue{});
        }
    }

    void widget(std::string_view name, reg::Id<glm::vec3> color_id)
    {
        ImGui::PushID(this);
        auto color = _variable_registries.get().get(color_id);
        if (color) {
            widget(name.data(), color_id, *color);
        }
        ImGui::PopID();
    }
    void widget(std::string_view, reg::Id<glm::vec2>)
    {
    }
    void widget(std::string_view, reg::Id<Cool::Camera>)
    {
    }

    void widget(std::string_view name, reg::Id<float> float_id, float current_value)
    {
        if (ImGui::SliderFloat(name.data(), &current_value, 0.f, 1.f)) {
            _command_executor.execute(
                Command_SetValue<float>{.id    = float_id,
                                        .value = current_value});
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            _command_executor.execute(
                Command_FinishedEditingValue{});
        }
    }

    void widget(std::string_view name, Input<glm::vec3>& color_slot)
    {
        ImGui::PushID(this);
        auto color = _variable_registries.get().get(color_slot.id);
        if (!color) {
            color         = glm::vec3{};
            color_slot.id = _variable_registries.get().create(*color);
        }
        widget(name, color_slot.id, *color);
        ImGui::PopID();
    }
    void widget(std::string_view, Input<Cool::Camera>&)
    {
    }
    void widget(std::string_view, Input<int>&)
    {
    }
    void widget(std::string_view, Input<glm::vec2>&)
    {
    }

    void widget(std::string_view name, Input<float>& float_slot)
    {
        ImGui::PushID(this);
        auto value = _variable_registries.get().get(float_slot.id);
        if (!value) {
            value         = float{};
            float_slot.id = _variable_registries.get().create(*value);
        }
        widget(name, float_slot.id, *value);
        ImGui::PopID();
    }

    void widget(std::string_view, Input_File& input)
    {
        input.update(_set_dirty); // TODO shouldn't be in the Ui, should be called all the frames even if we don't render the Ui
        std::string path = input.file_watcher.path().string();
        ImGui::Text("Path : ");
        ImGui::SameLine();
        ImGui::PushID(2132541);
        if (ImGui::InputText("", &path)) {
            input.file_watcher.set_path(path);
            _set_dirty(input._dirty_flag);
        }
        ImGui::PopID();
        ImGui::SameLine();
        if (Cool::ImGuiExtras::open_file_dialog(&path, Cool::NfdFileFilter::FragmentShader, Cool::File::whithout_file_name(path))) {
            input.file_watcher.set_path(path);
            _set_dirty(input._dirty_flag);
        }
    }

private:
    std::reference_wrapper<VariableRegistries> _variable_registries;
    CommandExecutor_TopLevel_Ref               _command_executor;
    SetDirtyFlag_Ref                           _set_dirty;
};

} // namespace Lab