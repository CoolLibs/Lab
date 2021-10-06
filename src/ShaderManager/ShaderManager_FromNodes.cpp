#include "ShaderManager_FromNodes.h"
#include <Cool/File/File.h>

ShaderManager_FromNodes::ShaderManager_FromNodes()
{
    _node_editor.subscribe_to_tree_change([this](NodeEditor& node_editor) {
        _fullscreen_pipeline.recompile_shader_from_source(node_editor.gen_raymarching_shader_code());
    });
}

void ShaderManager_FromNodes::ImGui_window()
{
    _node_editor.ImGui_window();
}