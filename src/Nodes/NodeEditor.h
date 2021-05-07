#pragma once

#include <imgui-node-editor/imgui_node_editor.h>

namespace ed = ax::NodeEditor;

struct LinkInfo {
	ed::LinkId Id;
	ed::PinId  InputId;
	ed::PinId  OutputId;
};

class NodeEditor {
public:
	NodeEditor();
	~NodeEditor();

	void ImGui_window();

private:
	ed::EditorContext* _context; 
	ImVector<LinkInfo> _links;
	int _next_link_id = 100; // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
};