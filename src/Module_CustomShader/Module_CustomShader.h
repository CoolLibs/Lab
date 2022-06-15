#pragma once
#include <Cool/File/File.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Path/Path.h>
#include "Dependencies/Input.h"
#include "Dependencies/Module.h"

namespace Lab {

class Module_CustomShader : public Module {
public:
    Module_CustomShader() = default;
    explicit Module_CustomShader(DirtyFlagFactory);

    void render(RenderParams) override;
    void imgui_windows(Ui ui) override;

    auto all_inputs()
    {
        return _inputs;
    }

    auto all_inputs() const -> AllInputRefsToConst override
    {
        AllInputRefsToConst inputs;
        inputs.push_back(AnyInputRefToConst{_camera_input});
        for (const auto& input : _inputs) {
            inputs.push_back(
                std::visit([](auto&& input) { return AnyInputRefToConst{input}; }, input));
        }
        return inputs;
    }

    auto is_dirty(DirtyManager dirty_manager) const -> bool override
    {
        return Module::is_dirty(dirty_manager) ||
               dirty_manager.is_dirty(_shader_is_dirty);
    }

private:
    void refresh_pipeline_if_necessary(InputProvider provider, DirtyManager dirty_manager, InputDestructorRef input_slot_destructor);
    void compile_shader(std::string_view fragment_shader_source_code, std::string_view shader_name);
    void parse_shader_for_params(std::string_view fragment_shader_source_code, InputDestructorRef input_slot_destructor);

private:
    Cool::FullscreenPipeline _fullscreen_pipeline{};
    std::vector<AnyInput>    _inputs;
    Input<Cool::Camera>      _camera_input;
    DirtyFlag                _shader_is_dirty; // Must be before _file because it is used to construct it
    Input_File               _file;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::base_class<Module>(this),
                _inputs,
                _camera_input,
                _shader_is_dirty,
                _file);
    }
};

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::Module_CustomShader);