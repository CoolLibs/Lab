#pragma once

#include <Cool/ImGui/ImGuiWindow.h>
#include "../ShaderManager/ShaderManager.h"
#include "NodeEditor.h"
#include "RenderEffects.h"

class Is0 : public ShaderManager {
public:
    void                             update() override;
    void                             imgui_windows() override;
    void                             on_key_pressed(const Cool::KeyboardEvent& event) override;
    void                             add_node(Node node);
    const std::vector<NodeTemplate>& nodes();

private:
    NodeEditor        _editor;
    RenderEffects     _effects;
    std::string       _shader_code;
    Cool::ImGuiWindow _shader_code_window{"is0 Shader Code", false};
    bool              _must_recompile = false;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Node Editor", _editor));
    }
};
