#include "ShaderManager_FromNodes.h"
#include <Cool/File/File.h>

ShaderManager_FromNodes::ShaderManager_FromNodes()
{
    // _node_editor.subscribe_to_tree_change([this](EdNodeEditor& node_editor) {
    //     _fullscreen_pipeline.compile(node_editor.gen_raymarching_shader_code(), "is0 Ray Marcher");
    // });
}

void ShaderManager_FromNodes::imgui_window()
{
    _node_editor.imgui_window();
    _fullscreen_pipeline.compile(_node_editor.shader_code(), "is0 Ray Marcher");
}