#include "ShaderManager_FromNodes.h"
#include <Cool/File/File.h>

ShaderManager_FromNodes::ShaderManager_FromNodes()
{
    _node_editor.subscribe_to_shader_code_changes([&](const std::string& shader_code) {
        _fullscreen_pipeline.compile(shader_code, "is0 Ray Marcher");
    });
}

void ShaderManager_FromNodes::imgui_window()
{
    _node_editor.imgui_window();
    _shader_code_window.show([&]() {
        ImGui::Text("%s", _node_editor.shader_code().c_str());
    });
    ImGui::Begin("is0 Opt");
    _shader_code_window.open_close_checkbox();
    ImGui::End();
}