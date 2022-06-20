#pragma once

#include <Cool/File/File.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/ImGui/ImGuiWindow.h>
#include <Cool/Serialization/as_json.h>
#include "Dependencies/Module.h"
#include "NodeEditor.h"
#include "RenderEffectsManager.h"
#include "RendererPBR.h"

namespace Lab {

class Module_is0 : public Module {
public:
    Module_is0() = default;
    Module_is0(DirtyFlagFactory_Ref, InputFactory_Ref);

    void                             update() override;
    void                             imgui_windows(Ui_Ref) const override;
    auto                             on_keyboard_event(const Cool::KeyboardEvent& event) -> bool override;
    void                             add_node(const Node& node) { _editor.add_node(node); }
    const std::vector<NodeTemplate>& nodes_templates() { return _editor.node_templates(); }
    std::string                      saving_path_string() const;
    auto                             all_inputs() const -> AllInputRefsToConst override;
    auto                             is_dirty(DirtyManager dirty_manager) const -> bool override;

protected:
    void render(RenderParams) override;

private:
    DirtyFlag                        _shader_is_dirty;
    Input<Cool::Camera>              _camera_input;
    mutable Cool::FullscreenPipeline _fullscreen_pipeline{};
    mutable NodeEditor               _editor{Cool::Path::root() + "/is0 nodes"};
    mutable RenderEffectsManager     _effects{Cool::Path::root() + "/is0 Render Effects"};
    mutable std::string              _shader_code;
    mutable Cool::ImGuiWindow        _shader_code_window{"is0 Shader Code", false};
    mutable bool                     _must_recompile = false; // TODO use a DirtyFlag instead
    CodeGen::LightProperties         _light;
    CodeGen::MaterialProperties      _material;
    int                              _in_use_render = 0;
    mutable std::string              _folder_path_for_save;
    mutable std::string              _file_name_for_save;

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
    //     if (!_file_name_for_save.empty()) {
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
        archive(cereal::base_class<Module>(this),
                cereal::make_nvp("Node Editor", _editor),
                cereal::make_nvp("Folder Path For Save", _folder_path_for_save),
                cereal::make_nvp("File Name For Save", _file_name_for_save),
                cereal::make_nvp("Render Effects Manager", _effects),
                cereal::make_nvp("Shader Dirty Flag", _shader_is_dirty),
                cereal::make_nvp("Camera Input", _camera_input));
    }
};

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::Module_is0)