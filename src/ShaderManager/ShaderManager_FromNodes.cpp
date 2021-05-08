#include "ShaderManager_FromNodes.h"

ShaderManager_FromNodes::ShaderManager_FromNodes() {
	_node_editor.subscribe_to_tree_change([this](NodeEditor& node_editor) {
		std::string source_code = node_editor.gen_raymarching_shader_code();
		Log::Warn(source_code);
		_shader.create_program({
			ShaderCode(ShaderType::Vertex, "Cool/Renderer_Fullscreen/fullscreen.vert"),
			ShaderCode::FromCode(ShaderType::Fragment, source_code),
		});
	});
}

void ShaderManager_FromNodes::ImGui_window() {
	_node_editor.ImGui_window();
}