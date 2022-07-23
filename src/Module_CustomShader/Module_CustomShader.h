#pragma once
#include <Cool/File/File.h>
#include <Cool/Path/Path.h>
#include "Cool/Dependencies/Input.h"
#include "Cool/Dependencies/InputFactory_Ref.h"
#include "Dependencies/Module.h"
#include "FullscreenShader.h"

namespace Lab {

class Module_CustomShader : public Module {
public:
    Module_CustomShader() = default;
    Module_CustomShader(Cool::DirtyFlagFactory_Ref, Cool::InputFactory_Ref);

    auto set_image_in_shader(std::string_view name, int slot, GLuint texture_id) -> void;

    auto render(RenderParams, UpdateContext_Ref) -> void override;
    void imgui_windows(Ui_Ref ui) const override;

    auto all_inputs() const -> Cool::AllInputRefsToConst override
    {
        Cool::AllInputRefsToConst inputs;
        inputs.push_back(Cool::AnyInputRefToConst{_camera_input});
        for (const auto& input : _inputs)
        {
            inputs.push_back(
                std::visit([](auto&& input) { return Cool::AnyInputRefToConst{input}; }, input)
            );
        }
        return inputs;
    }

    auto is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool override
    {
        return Module::is_dirty(check_dirty) ||
               check_dirty(_shader.dirty_flag());
    }

private:
    auto refresh_pipeline_if_necessary(
        Cool::InputProvider_Ref,
        Cool::IsDirty_Ref,
        Cool::InputFactory_Ref,
        Cool::InputDestructor_Ref,
        UpdateContext_Ref
    ) -> void;
    auto parse_shader_for_params(
        std::string_view fragment_shader_source_code,
        Cool::InputFactory_Ref,
        Cool::InputDestructor_Ref
    ) -> void;

private:
    FullscreenShader            _shader; // Must be before _file because it is used to construct it
    std::vector<Cool::AnyInput> _inputs;
    Cool::Input<Cool::Camera>   _camera_input;
    mutable Cool::Input_File    _file;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    auto serialize(Archive& archive) -> void
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