#if DEBUG

#include "TestPresets.h"

namespace Lab {

void TestPresets::imgui_window()
{
    ImGui::Begin("Test Presets");
    _manager.imgui(_current_settings);
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();
    if (ImGui::Button("Add a field in the settings"))
    {
        _current_settings.push_back(Cool::Variable<int>{.name = std::to_string(_next_field_name++), .value = 5});
    }
    if (ImGui::Button("Remove a field in the settings"))
    {
        if (!_current_settings.empty())
        {
            _current_settings.pop_back();
        }
    }
    if (ImGui::Button("Add and edit a preset"))
    {
        const auto test_id = _manager.add({.name = "Test", .values = _current_settings});
        if (_manager.contains(test_id)) // Check if the adding was successful
        {
            _manager.edit(test_id, {
                                       Cool::Variable<int>{.name = "First", .value = 0},
                                       Cool::Variable<float>{.name = "Second", .value = 0.f},
                                       Cool::Variable<float>{.name = "Third", .value = 0.f},
                                   });
        }
    }
    ImGui::End();
}

} // namespace Lab

#endif