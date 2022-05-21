#pragma once
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/NfdFileFilter/NfdFileFilter.h>
#include "CommandsExec.h"
#include "InputSlot.h"
#include "Registries.h"

namespace Lab {

class Ui {
public:
    Ui(Registries& registries, CommandDispatcher commands, SetDirtyFlag set_dirty)
        : _registries{registries}, _commands{commands}, _set_dirty{set_dirty}
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
            _commands.dispatch(Command_SetValue<glm::vec3>{.id    = color_id,
                                                           .value = current_value});
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            _commands.dispatch(Command_FinishedEditingValue{});
        }
    }

    void widget(std::string_view name, reg::Id<glm::vec3> color_id)
    {
        ImGui::PushID(this);
        auto color = _registries.get().get(color_id);
        if (color) {
            widget(name.data(), color_id, *color);
        }
        ImGui::PopID();
    }

    void widget(std::string_view name, reg::Id<float> float_id, float current_value)
    {
        if (ImGui::SliderFloat(name.data(), &current_value, 0.f, 1.f)) {
            _commands.dispatch(Command_SetValue<float>{.id    = float_id,
                                                       .value = current_value});
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            _commands.dispatch(Command_FinishedEditingValue{});
        }
    }

    void widget(std::string_view name, InputSlot<glm::vec3>& color_slot)
    {
        ImGui::PushID(this);
        auto color = _registries.get().get(color_slot.id);
        if (!color) {
            color         = glm::vec3{};
            color_slot.id = _registries.get().create(*color);
        }
        widget(name, color_slot.id, *color);
        ImGui::PopID();
    }

    void widget(std::string_view name, InputSlot<float>& float_slot)
    {
        ImGui::PushID(this);
        auto value = _registries.get().get(float_slot.id);
        if (!value) {
            value         = float{};
            float_slot.id = _registries.get().create(*value);
        }
        widget(name, float_slot.id, *value);
        ImGui::PopID();
    }

    void widget(std::string_view name, InputSlot_File& slot)
    {
        slot.update(_set_dirty); // TODO shouldn't be in the Ui, should be called all the frames even if we don't render the Ui
        std::string path = slot.file_watcher.path().string();
        ImGui::Text("Path : ");
        ImGui::SameLine();
        ImGui::PushID(2132541);
        if (ImGui::InputText("", &path)) {
            slot.file_watcher.set_path(path);
            _set_dirty(slot._dirty_flag);
        }
        ImGui::PopID();
        ImGui::SameLine();
        if (Cool::ImGuiExtras::open_file_dialog(&path, Cool::NfdFileFilter::FragmentShader, Cool::File::whithout_file_name(path))) {
            slot.file_watcher.set_path(path);
            _set_dirty(slot._dirty_flag);
        }
    }

private:
    std::reference_wrapper<Registries> _registries;
    CommandDispatcher                  _commands;
    SetDirtyFlag                       _set_dirty;
};

} // namespace Lab