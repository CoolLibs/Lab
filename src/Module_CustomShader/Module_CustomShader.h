#pragma once
#include <Cool/Dependencies/Input.h>
#include <Cool/Dependencies/InputFactory_Ref.h>
#include <Cool/File/File.h>
#include <Cool/Log/MessageSender.h>
#include <Cool/Path/Path.h>
#include <Cool/Presets/PresetManager.h>
#include "Common/FullscreenShader.h"
#include "Dependencies/Module.h"

namespace Lab {

class Module_CustomShader : public Module {
public:
    Module_CustomShader() = default;
    Module_CustomShader(Cool::DirtyFlagFactory_Ref, Cool::InputFactory_Ref);

    void set_image_in_shader(std::string_view name, int slot, GLuint texture_id);

    void render(RenderParams, UpdateContext_Ref) override;
    void imgui_windows(Ui_Ref ui) const override;

    auto all_inputs() const -> Cool::AllInputRefsToConst override
    {
        Cool::AllInputRefsToConst all_inputs;
        all_inputs.push_back(Cool::AnyInputRefToConst{_camera_input});
        for (const auto& input : inputs())
        {
            all_inputs.push_back(
                std::visit([](auto&& input) { return Cool::AnyInputRefToConst{input}; }, input)
            );
        }
        return all_inputs;
    }

    auto is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool override
    {
        return Module::is_dirty(check_dirty) ||
               check_dirty(_shader.dirty_flag());
    }

private:
    void refresh_pipeline_if_necessary(
        Cool::InputProvider_Ref,
        Cool::IsDirty_Ref,
        Cool::InputFactory_Ref,
        Cool::InputDestructor_Ref,
        UpdateContext_Ref,
        Cool::VariableRegistries&
    );
    void parse_shader_for_params(
        std::string_view fragment_shader_source_code,
        Cool::InputFactory_Ref,
        Cool::InputDestructor_Ref
    );
    void apply_first_preset_if_there_is_one(Cool::VariableRegistries& variable_registries);

    auto inputs() const -> std::vector<Cool::AnyInput>& { return _inputs; }
    auto inputs() -> std::vector<Cool::AnyInput>& { return _inputs; }

private:
    FullscreenShader          _shader; // Must be before _file because it is used to construct it
    Cool::MessageSender       _shader_compilation_error{};
    Cool::Input<Cool::Camera> _camera_input;
    mutable Cool::Input_File  _file;
    std::filesystem::path     _previous_path;
    // Cool::MessageSender                         _parsing_error_message{}; // TODO(JF) Use this
    mutable std::optional<Cool::PresetManager> _presets_manager{};
    mutable std::vector<Cool::AnyInput>        _inputs{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Base Module", cereal::base_class<Module>(this)),
            cereal::make_nvp("Inputs", inputs()),
            cereal::make_nvp("Camera Input", _camera_input),
            cereal::make_nvp("Shader", _shader),
            cereal::make_nvp("File", _file)
        );
    }
};

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::Module_CustomShader);
