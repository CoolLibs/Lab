#pragma once
#include <Cool/File/File.h>
#include <Cool/Path/Path.h>
#include "Dependencies/Input.h"
#include "Dependencies/InputFactory_Ref.h"
#include "Dependencies/Module.h"
#include "FullscreenShader.h"

namespace Lab {

class Module_CustomShader : public Module {
public:
    Module_CustomShader() = default;
    Module_CustomShader(DirtyFlagFactory_Ref, InputFactory_Ref);

    void set_image_in_shader(std::string_view name, int slot, GLuint texture_id);

    void render(RenderParams, UpdateContext_Ref) override;
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

    auto is_dirty(IsDirty_Ref check_dirty) const -> bool override
    {
        return Module::is_dirty(check_dirty) ||
               check_dirty(_shader.dirty_flag());
    }

private:
    void refresh_pipeline_if_necessary(InputProvider_Ref, IsDirty_Ref, InputFactory_Ref, InputDestructor_Ref, UpdateContext_Ref);
    void parse_shader_for_params(std::string_view fragment_shader_source_code, InputFactory_Ref, InputDestructor_Ref);

private:
    FullscreenShader      _shader; // Must be before _file because it is used to construct it
    std::vector<AnyInput> _inputs;
    Input<Cool::Camera>   _camera_input;
    mutable Input_File    _file;

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
            cereal::make_nvp("Shader", _shader),
            cereal::make_nvp("File", _file)
        );
    }
};

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::Module_CustomShader);