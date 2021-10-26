#include "NodeEditor.h"
#include "CodeGen.h"

NodeEditor::NodeEditor()
{
    update_shader_code();
}

void NodeEditor::subscribe_to_shader_code_changes(std::function<void(const std::string&)> callback)
{
    _on_shader_code_change.push_back(callback);
    callback(_shader_code);
}

void NodeEditor::update_shader_code()
{
    _shader_code = CodeGen::full_shader_code(_nodes, _links, _factory.templates());
    for (const auto& callback : _on_shader_code_change) {
        callback(_shader_code);
    }
}

static void show_node_pins(const Node& node)
{
    ImGui::BeginGroup();
    for (const auto& pin : node.input_pins) {
        pin.show();
    }
    ImGui::EndGroup();
    ImGui::SameLine(200.f);
    node.output_pin.show();
}

static void show_node_params(Node& node, std::function<void()> on_change)
{
    ImGui::BeginGroup();
    ImGui::PushID(&node);
    ImGui::PushItemWidth(200.f);
    node.parameter_list.imgui(on_change);
    ImGui::PopItemWidth();
    ImGui::PopID();
    ImGui::EndGroup();
}

static void show_node(Node& node, std::function<void()> on_change)
{
    ed::BeginNode(static_cast<ed::NodeId>(node.uuid));
    {
        ImGui::Text("%s", node.node_template_name.c_str());
        show_node_pins(node);
        show_node_params(node, on_change);
    }
    ed::EndNode();
}

static void show_link(const Link& link)
{
    ed::Link(link.id, link.from_pin_id, link.to_pin_id);
}

static const Pin& find_pin(ed::PinId id, const std::vector<Node>& nodes)
{
    for (const auto& node : nodes) {
        if (node.output_pin.id() == id) {
            return node.output_pin;
        }
        for (const auto& pin : node.input_pins) {
            if (pin.id() == id) {
                return pin;
            }
        }
    }
    throw std::invalid_argument("pin not found");
}

void NodeEditor::handle_link_creation()
{
    if (ed::BeginCreate()) {
        ed::PinId from_pin_id, to_pin_id;
        if (ed::QueryNewLink(&from_pin_id, &to_pin_id)) {
            if (from_pin_id && to_pin_id && ed::AcceptNewItem()) {
                const Pin* from_pin = &find_pin(from_pin_id, _nodes);
                const Pin* to_pin   = &find_pin(to_pin_id, _nodes);

                if (from_pin->kind() == ed::PinKind::Input) // Reorder so that we always go from an output pin to an input pin
                {
                    std::swap(from_pin, to_pin);
                    std::swap(from_pin_id, to_pin_id);
                }

                bool accept_link = true;
                // Check that there isn't already a node connected to the end_pin
                // accept_link &= !_registry.valid(compute_node_connected_to_pin(end_pin_id));
                // Check that one pin is an input and the other an output
                accept_link &= from_pin->kind() != to_pin->kind();
                // Check that we are not linking a node to itself
                // accept_link &= start_pin_info.node_entity != end_pin_info.node_entity;

                if (accept_link) {
                    _links.push_back(Link{.from_pin_id = from_pin_id,
                                          .to_pin_id   = to_pin_id});
                    // _registry.remove_if_exists<IsTerminalNode>(start_pin_info.node_entity);
                    update_shader_code();
                    // Draw new link
                    //ed::Link(_links.back().id, _links.back().start_pin_id, _links.back().end_pin_id);
                }
                else {
                    ed::RejectNewItem();
                }
            }
        }
    }
    ed::EndCreate(); // Wraps up object creation action handling.
}

void NodeEditor::imgui_window()
{
    ImGui::Begin("is0");
    ed::SetCurrentEditor(&*_context);
    ed::Begin("My Editor");
    {
        for (auto& node : _nodes) {
            show_node(node, [&]() { update_shader_code(); });
        }
        for (const auto& link : _links) {
            show_link(link);
        }
        handle_link_creation();
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
        update_shader_code();
        return true;
    }
    else {
        return false;
    }
}