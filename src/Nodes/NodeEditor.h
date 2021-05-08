#pragma once

#include <imgui-node-editor/imgui_node_editor.h>
#include <entt/entt.hpp>

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

	std::string gen_raymarching_shader_code();

	void subscribe_to_tree_change(std::function<void(NodeEditor&)> callback);

private:
	void on_tree_change();
	std::string gen_scene_sdf();

private:
	ed::EditorContext* _context; 
	ImVector<LinkInfo> _links;
	int _next_link_id = 100; // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
	std::vector<std::function<void(NodeEditor&)>> _on_tree_change_callbacks;
	entt::registry _registry;
};