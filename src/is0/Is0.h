#pragma once

#include <Cool/File/File.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/ImGui/ImGuiWindow.h>
#include "../ShaderManager/ShaderManager.h"
#include "NodeEditor.h"
#include "RenderEffects.h"
#include "RendererPBR.h"

class Is0 : public ShaderManager {
public:
    void                             update() override;
    void                             imgui_windows() override;
    void                             on_key_pressed(const Cool::KeyboardEvent& event) override;
    void                             add_node(const Node& node) { _editor.add_node(node); }
    const std::vector<NodeTemplate>& nodes_templates() { return _editor.node_templates(); }

private:
    NodeEditor                  _editor{Cool::Path::root() + "/is0 nodes"};
    RenderEffects               _effects;
    std::string                 _shader_code;
    Cool::ImGuiWindow           _shader_code_window{"is0 Shader Code", false};
    bool                        _must_recompile = false;
    CodeGen::LightProperties    _light;
    CodeGen::MaterialProperties _material;
    int                         _in_use_render = 0;
    std::string                 _folder_path_for_save;
    std::string                 _path_for_load;
    std::string                 _file_name_for_save;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Node Editor", _editor));
    }
};
