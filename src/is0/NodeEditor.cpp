#include "NodeEditor.h"
#include <Cool/Log/ToUser.h>
#include <Cool/Parameter/ParameterU.h>
#include <glfw/glfw3.h>
#include "CodeGen.h"
#include "NodeEditorU.h"

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
    if (_all_nodes_have_a_valid_template) {
        _shader_code = CodeGen::full_shader_code(_tree.nodes, _tree.links, _factory.templates());
    }
    else {
        _shader_code = "void main() { gl_FragColor = vec4(vec3(0.), 1.); }";
    }
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
    ImNodes::BeginNode(node.id);
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(node.node_template_name.c_str());
    ImNodes::EndNodeTitleBar();
    show_node_pins(node);
    show_node_params(node, on_change);
    ImNodes::EndNode();
}

static void show_link(const Link& link)
{
    ImNodes::Link(link.id, link.from_pin_id, link.to_pin_id);
}

static const Pin& find_pin(PinId id, const std::vector<Node>& nodes)
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
    int from_pin_id, to_pin_id;
    if (ImNodes::IsLinkCreated(&from_pin_id, &to_pin_id)) {
        _tree.delete_link_going_to(PinId{to_pin_id});
        _tree.add_link(Link{.from_pin_id = PinId{from_pin_id},
                            .to_pin_id   = PinId{to_pin_id}});
        update_shader_code();
    }
}

void NodeEditor::handle_link_deletion()
{
    int  link_id;
    bool has_deleted_some = false;
    if (ImNodes::IsLinkDestroyed(&link_id)) {
        _tree.delete_link(LinkId{link_id});
        has_deleted_some = true;
    }

    const int num_selected = ImNodes::NumSelectedLinks();
    if (num_selected > 0 && ImGui::IsKeyReleased(GLFW_KEY_DELETE)) {
        has_deleted_some = true;
        static std::vector<int> selected_links;
        selected_links.resize(static_cast<size_t>(num_selected));
        ImNodes::GetSelectedLinks(selected_links.data());
        for (const int link_id : selected_links) {
            _tree.delete_link(LinkId{link_id});
        }
    }
    if (has_deleted_some) {
        update_shader_code();
    }
}

void NodeEditor::handle_node_deletion()
{
    const int num_selected = ImNodes::NumSelectedNodes();
    if (num_selected > 0 && ImGui::IsKeyReleased(GLFW_KEY_DELETE)) {
        static std::vector<int> selected_nodes;
        selected_nodes.resize(static_cast<size_t>(num_selected));
        ImNodes::GetSelectedNodes(selected_nodes.data());
        for (const int node_id : selected_nodes) {
            _tree.delete_node(NodeId{node_id});
        }
        update_shader_code();
    }
}

void NodeEditor::imgui_window()
{
    ImGui::Begin("is0");
    ImNodes::BeginNodeEditor();

    // static float col[3] = {1, 1, 0};
    // ImGui::ColorEdit3("col", col);
    // ImGui::Dummy(ImVec2(80.0f, 45.0f));
    ImNodes::MiniMap();

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("_node_templates_list");
    }
    if (ImGui::BeginPopup("_node_templates_list")) {
        imgui_make_node();
        ImGui::EndPopup();
    }
    {
        for (auto& node : _tree.nodes) {
            show_node(node, [&]() { update_shader_code(); });
        }
        for (const auto& link : _tree.links) {
            show_link(link);
        }
    }
    ImNodes::EndNodeEditor();
    handle_link_creation();
    handle_link_deletion();
    handle_node_deletion();
    ImGui::End();
}

bool NodeEditor::imgui_make_node()
{
    const std::optional<Node> node = _factory.imgui();
    if (node.has_value()) {
        _tree.add_node(*node);
        ImNodes::SetNodeScreenSpacePos(node->id, ImGui::GetMousePos());
        update_shader_code();
        return true;
    }
    else {
        return false;
    }
}

void NodeEditor::update_templates_and_nodes()
{
    _all_nodes_have_a_valid_template = true;
    _factory.reload_templates();
    for (auto& node : _tree.nodes) {
        const auto node_template = std::ranges::find_if(_factory.templates(), [&](const NodeTemplate& node_template) {
            return node_template.name == node.node_template_name;
        });
        if (node_template == _factory.templates().end()) {
            _all_nodes_have_a_valid_template = false;
            Cool::Log::ToUser::warn("is0 " + node.node_template_name, "Can't find node file '{0}.is0' Your graph can't be compiled.\nEither add a '{0}.is0' file or delete all {0} nodes.", node.node_template_name);
        }
        else {
            // Update params
            Cool::ParameterList new_parameters{};
            new_parameters->reserve(node.parameter_list->size());
            for (const auto& desc : node_template->parameters) {
                new_parameters->push_back(Cool::ParameterU::make_param(node.parameter_list, desc));
            }
            node.parameter_list = std::move(new_parameters);
            // Update input pins
            const auto nb_pins = node_template->sdf_identifiers.size();
            if (nb_pins < node.input_pins.size()) {
                for (size_t i = nb_pins; i < node.input_pins.size(); ++i) {
                    _tree.delete_link_going_to(node.input_pins[i].id());
                }
            }
            node.input_pins.resize(nb_pins);
        }
    }
    update_shader_code();
}