#pragma once

#include <Cool/Variables/AnyVariable.h>
#include <Cool/Variables/Presets.h>

namespace Lab {

class TestPresets {
public:
    TestPresets()
    {
        // TODO add some presets to the manager
        std::vector<Cool::AnyVariable> values{
            Cool::Variable<int>{.name = "Premier", .value = 3},
            Cool::Variable<float>{.name = "Second", .value = 1.f},
            Cool::Variable<float>{.name = "Troisième", .value = 1.f},
        };

        Cool::Preset preset = {
            .name   = "test",
            .values = values};

        Cool::Preset preset2 = {
            .name   = "test2",
            .values = values};

        Cool::Preset preset3 = {
            .name   = "test3",
            .values = values};

        Cool::Preset preset4 = {
            .name   = "test4",
            .values = values};

        _manager.add_preset(preset);
        _manager.add_preset(preset2);
        _manager.add_preset(preset3);
        _manager.add_preset(preset4);
    }
    void imgui_window()
    {
        ImGui::Begin("Test Presets");
        // TODO call imgui for the presetManager3
        _manager.imgui(values);
        ImGui::End();
    }

private:
    // TODO Put a PresetManager here
    Cool::PresetManager _manager;

    Cool::PresetData values{
        Cool::Variable<int>{.name = "Premier", .value = 3},
        Cool::Variable<float>{.name = "Second", .value = 1.f},
        Cool::Variable<float>{.name = "Troisième", .value = 1.f},
    };
};

} // namespace Lab
