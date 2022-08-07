#pragma once
#if DEBUG

#include <Cool/Variables/Presets.h>

namespace Lab {

class TestPresets {
public:
    void imgui_window()
    {
        ImGui::Begin("Test Presets");
        _manager.imgui(_current_settings);
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