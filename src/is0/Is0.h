#pragma once

#include <Cool/File/File.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/ImGui/ImGuiWindow.h>
#include <Cool/Serialization/as_json.h>
#include "../ShaderManager/ShaderManager.h"
#include "NodeEditor.h"
#include "RenderEffectsManager.h"
#include "RendererPBR.h"

class Is0 : public ShaderManager {
public:
    void                             update() override;
    void                             imgui_windows() override;
    void                             on_key_pressed(const Cool::KeyboardEvent& event) override;
    void                             add_node(const Node& node) { _editor.add_node(node); }
    const std::vector<NodeTemplate>& nodes_templates() { return _editor.node_templates(); }
    std::string                      saving_path_string() const;

private:
    NodeEditor                  _editor{Cool::Path::root() + "/is0 nodes"};
    RenderEffectsManager        _effects{Cool::Path::root() + "/is0 Render Effects"};
    std::string                 _shader_code;
    Cool::ImGuiWindow           _shader_code_window{"is0 Shader Code", false};
    bool                        _must_recompile = false;
    CodeGen::LightProperties    _light;
    CodeGen::MaterialProperties _material;
    int                         _in_use_render = 0;
    std::string                 _folder_path_for_save;
    std::string                 _file_name_for_save;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(cereal::make_nvp("Node Editor", _editor),
                cereal::make_nvp("Folder Path For Save", _folder_path_for_save),
                cereal::make_nvp("File Name For Save", _file_name_for_save),
                cereal::make_nvp("Render Effects Manager", _effects));
        if (!_file_name_for_save.empty()) {
            const std::string      saving_path = saving_path_string();
            const std::string      message     = "Do you want to save your work in " + saving_path + "?";
            const boxer::Selection sel         = boxer::show(message.c_str(), "Save", boxer::Style::Question, boxer::Buttons::YesNo);
            if (sel == boxer::Selection::Yes) {
                Cool::Serialization::to_json(_editor, saving_path);
            }
        }
    }
    template<class Archive>
    void load(Archive& archive)
    {
        archive(cereal::make_nvp("Node Editor", _editor),
                cereal::make_nvp("Folder Path For Save", _folder_path_for_save),
                cereal::make_nvp("File Name For Save", _file_name_for_save),
                cereal::make_nvp("Render Effects Manager", _effects));
    }
};