#pragma once
#include <Cool/File/File.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Path/Path.h>
#include "Dependencies/Input.h"
#include "Dependencies/InputFactory_Ref.h"
#include "Dependencies/Module.h"

namespace Lab {

class Module_CustomShader : public Module {
public:
    Module_CustomShader() = default;
    Module_CustomShader(DirtyFlagFactory_Ref, InputFactory_Ref);

    void set_image_in_shader(std::string_view name, int slot, GLuint texture_id);

    void render(RenderParams) override;
    void imgui_windows(Ui_Ref ui) const override;

    auto all_inputs() const -> AllInputRefsToConst override
    {
        AllInputRefsToConst inputs;
        inputs.push_back(AnyInputRefToConst{_camera_input});
        for (const auto& input : _inputs)
        {
            inputs.push_back(
                std::visit([](auto&& input) { return AnyInputRefToConst{input}; }, input)
            );
        }
        return inputs;
    }

    auto is_dirty(DirtyManager dirty_manager) const -> bool override
    {
        return Module::is_dirty(dirty_manager) ||
               dirty_manager.is_dirty(_shader_is_dirty);
    }

private:
    void refresh_pipeline_if_necessary(InputProvider, DirtyManager, InputFactory_Ref, InputDestructor_Ref);
    void compile_shader(std::string_view fragment_shader_source_code, std::string_view shader_name);
    void parse_shader_for_params(std::string_view fragment_shader_source_code, InputFactory_Ref, InputDestructor_Ref);

private:
    Cool::FullscreenPipeline _fullscreen_pipeline{};
    std::vector<AnyInput>    _inputs;
    Input<Cool::Camera>      _camera_input;
    DirtyFlag                _shader_is_dirty; // Must be before _file because it is used to construct it
    mutable Input_File       _file;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Base Module", cereal::base_class<Module>(this)),
            cereal::make_nvp("Inputs", _inputs),
            cereal::make_nvp("Camera Input", _camera_input),
            cereal::make_nvp("Shader Dirty Flag", _shader_is_dirty),
            cereal::make_nvp("File", _file)
        );
    }
};

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::Module_CustomShader);