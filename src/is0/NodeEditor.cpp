#include "NodeEditor.h"
#include "CodeGen.h"

static void show_node(Node& node)
{
    ed::BeginNode(static_cast<ed::NodeId>(node.uuid));
    {
        ImGui::Text("%s", node.node_template_name.c_str());
        ImGui::BeginGroup();
        for (const auto& pin : node.input_pins) {
            pin.show();
        }
        ImGui::EndGroup();
        ImGui::SameLine(200.f);
        node.output_pin.show();

        ImGui::BeginGroup();
        ImGui::PushID(&node);
        ImGui::PushItemWidth(200.f);
        node.parameter_list.imgui();
        ImGui::PopItemWidth();
        ImGui::PopID();
        // if () {
        //     // on_tree_change();
        // }
        ImGui::EndGroup();
    }
    ed::EndNode();
}

static void show_link(const Link& link)
{
    ed::Link(link.id, link.from_pin_id, link.to_pin_id);
}

void NodeEditor::imgui_window()
{
    ImGui::Begin("Nodes");
    ed::SetCurrentEditor(&*_context);
    ed::Begin("My Editor");
    {
        for (auto& node : _nodes) {
            show_node(node);
        }
        for (const auto& link : _links) {
            show_link(link);
        }
    }
    ed::End();
    if (ImGui::BeginPopupContextItem("_node_templates_list", ImGuiPopupFlags_MouseButtonMiddle)) {
        imgui_make_node();
        ImGui::EndPopup();
    }
    ImGui::End();
}

bool NodeEditor::imgui_make_node()
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