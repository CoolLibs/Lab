#pragma once

#include <Cool/File/File.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/ImGui/ImGuiWindow.h>
#include <Cool/Serialization/as_json.h>
#include "Dependencies/Module.h"
#include "FullscreenShader.h"
#include "NodeEditor.h"
#include "RenderEffectsManager.h"
#include "RendererPBR.h"

namespace Lab {

class Module_is0 : public Module {
public:
    Module_is0() = default;
    Module_is0(Cool::DirtyFlagFactory_Ref, Cool::InputFactory_Ref);

    void                             update(Cool::UpdateContext_Ref) override;
    void                             imgui_windows(Cool::Ui_Ref) const override;
    void                             add_node(const Node& node) { _editor.add_node(node); }
    const std::vector<NodeTemplate>& nodes_templates() { return _editor.node_templates(); }
    std::string                      saving_path_string() const;
    auto                             all_inputs() const -> Cool::AllInputRefsToConst override;
    auto                             is_dirty(Cool::IsDirty_Ref) const -> bool override;

protected:
    void render(RenderParams, Cool::UpdateContext_Ref) override;

private:
    // TODO remove all those `mutable` once ui function is done properly

    FullscreenShader             _shader;
    Cool::Input<Cool::Camera>    _camera_input;
    mutable NodeEditor           _editor{Cool::Path::root() + "/is0 nodes"};
    mutable RenderEffectsManager _effects{Cool::Path::root() + "/is0 Render Effects"};
    mutable std::string          _shader_code;
    mutable Cool::ImGuiWindow    _shader_code_window{"is0 Shader Code", false};
    mutable bool                 _must_recompile              = false; // TODO use a DirtyFlag instead (which is alreadt in FullscreenShader)
    mutable bool                 _must_regenerate_shader_code = false; // TODO use a DirtyFlag instead (which is alreadt in FullscreenShader)
    CodeGen::LightProperties     _light;
    CodeGen::MaterialProperties  _material;
    int                          _in_use_render = 0;
    mutable std::string          _folder_path_for_save;
    mutable std::string          _file_name_for_save;

private:
    // Serialization
    friend class cereal::access;
    // template<class Archive>
    // void save(Archive& archive) const
    // {
    //     archive(cereal::base_class<Module>(this),
    //             cereal::make_nvp("Node Editor", _editor),
    //             cereal::make_nvp("Folder Path For Save", _folder_path_for_save),
    //             cereal::make_nvp("File Name For Save", _file_name_for_save),
    //             cereal::make_nvp("Render Effects Manager", _effects));
    //     if (!_file_name_for_save.empty()) { // TODO(JF) should be done in a separate type so that is0 can have a single `serialize` function. Also, we should be saving the whole project, not just is0.
    //         const std::string      saving_path = saving_path_string();
    //         const std::string      message     = "Do you want to save your work in " + saving_path + "?";
    //         const boxer::Selection sel         = boxer::show(message.c_str(), "Save", boxer::Style::Question, boxer::Buttons::YesNo);
    //         if (sel == boxer::Selection::Yes) {
    //             Cool::Serialization::to_json(_editor, saving_path);
    //         }
    //     }
    // }
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Base Module", cereal::base_class<Module>(this)),
            cereal::make_nvp("Node Editor", _editor),
            cereal::make_nvp("Folder Path For Save", _folder_path_for_save),
            cereal::make_nvp("File Name For Save", _file_name_for_save),
            cereal::make_nvp("Render Effects Manager", _effects),
            cereal::make_nvp("Shader", _shader),
            cereal::make_nvp("Camera Input", _camera_input)
        );
    }
};

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::Module_is0)