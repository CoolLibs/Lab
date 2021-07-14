#pragma once

#include <imgui-node-editor/imgui_node_editor.h>
#include <entt/entt.hpp>

namespace ed = ax::NodeEditor;

struct LinkInfo {
    ed::LinkId   id;
    ed::PinId    start_pin_id;
    entt::entity start_node_entity;
    ed::PinId    end_pin_id;
    entt::entity end_node_entity;
};

struct PinInfo {
    ed::PinKind  kind;
    entt::entity node_entity = entt::null;
};

class NodeEditor {
public:
    NodeEditor();
    ~NodeEditor();

    void ImGui_window();

    std::string gen_raymarching_shader_code();

    void subscribe_to_tree_change(std::function<void(NodeEditor&)> callback);

    /// <summary>
    ///
    /// </summary>
    /// <param name="pin_id"></param>
    /// <returns>Can return entt::null if no node is connected to the pin</returns>
    entt::entity compute_node_connected_to_pin(ed::PinId pin_id);

private:
    void        on_tree_change();
    std::string gen_scene_sdf();
    PinInfo     compute_pin_infos(ed::PinId pin_id);

private:
    ed::EditorContext*                            _context;
    ImVector<LinkInfo>                            _links;
    int                                           _next_link_id = 100; // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
    std::vector<std::function<void(NodeEditor&)>> _on_tree_change_callbacks;
    entt::registry                                _registry;
};