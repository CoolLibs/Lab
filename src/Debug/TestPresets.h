#pragma once
#if DEBUG

#include <Cool/Variables/PresetManager.h>

// TODO(JF) Move to Cool (after merging main, after Jules merges their branch)
// TODO(JF) Use a Window DebugOption
// TODO(JF) Move to a .cpp
namespace Lab {

class TestPresets {
public:
    void imgui_window()
    {
        ImGui::Begin("Test Presets");
        _manager.imgui(_current_settings);
        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();
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

private:
    Cool::PresetManager _manager{Cool::Path::root() + "/test-presets-manager.json"};

    Cool::Settings _current_settings{
        Cool::Variable<int>{.name = "First", .value = 3},
        Cool::Variable<float>{.name = "Second", .value = 1.f},
        Cool::Variable<float>{.name = "Third", .value = 1.f},
    };
};

} // namespace Lab

#endif