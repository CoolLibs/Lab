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

        std::vector<Cool::AnyVariable> values2{
            Cool::Variable<int>{.name = "Premier", .value = 2},
            Cool::Variable<float>{.name = "Second", .value = .5f},
            Cool::Variable<float>{.name = "Troisième", .value = 1.2f},
        };

        std::vector<Cool::AnyVariable> values3{
            Cool::Variable<int>{.name = "Premier", .value = 12},
            Cool::Variable<float>{.name = "Second", .value = 0.8f},
            Cool::Variable<float>{.name = "Troisième", .value = 4.2f},
        };

        std::vector<Cool::AnyVariable> values4{
            Cool::Variable<int>{.name = "Premier", .value = 9},
            Cool::Variable<float>{.name = "Second", .value = 2.4f},
            Cool::Variable<float>{.name = "Troisième", .value = 0.7f},
        };

        Cool::Preset2 preset = {
            .name   = "test",
            .values = values};

        Cool::Preset2 preset2 = {
            .name   = "test2",
            .values = values2};

        Cool::Preset2 preset3 = {
            .name   = "test3",
            .values = values3};

        Cool::Preset2 preset4 = {
            .name   = "test4",
            .values = values4};

        // _manager.add_preset(preset);
        // _manager.add_preset(preset2);
        // _manager.add_preset(preset3);
        // _manager.add_preset(preset4);
    }
    void imgui_window()
    {
        ImGui::Begin("Test Presets");
        // TODO call imgui for the presetManager3
        _manager.imgui(values);
        ImGui::End();
    }

private:
    Cool::PresetManager _manager{Cool::Path::root() + "/test-presets-manager.json"};

    Cool::PresetData values{
        Cool::Variable<int>{.name = "Premier", .value = 3},
        Cool::Variable<float>{.name = "Second", .value = 1.f},
        Cool::Variable<float>{.name = "Troisième", .value = 1.f},
    };
};

} // namespace Lab
