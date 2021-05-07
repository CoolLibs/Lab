#pragma once

#include <imgui-node-editor/imgui_node_editor.h>

namespace ed = ax::NodeEditor;

class NodeEditor {
public:
	NodeEditor();
	~NodeEditor();

	void ImGui_window();

private:
	ed::EditorContext* _context;
};