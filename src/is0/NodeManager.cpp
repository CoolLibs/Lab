#include "NodeManager.h"
#include "CodeGen.h"

static void show_node(Node& node)
{
    ed::BeginNode(static_cast<ed::NodeId>(node.uuid));
    {
        ImGui::Text("%s", node.node_template_name.c_str());
        ImGui::BeginGroup();
        node.parameter_list.imgui();
        // if () {
        //     // on_tree_change();
        // }
        ImGui::EndGroup();
        // ImGui::SameLine();
        // ed::BeginPin(shape_node.output_pin.id, ed::PinKind::Output);
        // ImGui::Text("OUT->");
        // ed::EndPin();
    }
    ed::EndNode();
}

void NodeManager::imgui_window()
{
    ImGui::Begin("Nodes");
    ed::SetCurrentEditor(&*_context);
    ed::Begin("My Editor");
    {
        for (auto& node : _nodes) {
            show_node(node);
        }
    }
    ed::End();
    if (ImGui::BeginPopupContextItem("_node_templates_list", ImGuiPopupFlags_MouseButtonMiddle)) {
        imgui_make_node();
        ImGui::EndPopup();
    }
    ImGui::End();
}

bool NodeManager::imgui_make_node()
{
    const std::optional<Node> node = _factory.imgui();
    if (node.has_value()) {
        _nodes.push_back(*node);
        return true;
    }
    else {
        return false;
    }
}