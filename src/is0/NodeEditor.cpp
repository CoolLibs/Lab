#include "NodeEditor.h"
#include <Cool/Log/ToUser.h>
#include <Cool/Parameter/ParameterU.h>
#include <GLFW/glfw3.h>
#include "Randomizer.h"

NodeEditor::NodeEditor(std::string_view nodes_folder_path)
    : _factory{nodes_folder_path}
{
}

bool NodeEditor::tree_has_changed()
{
    bool b            = _tree_has_changed;
    _tree_has_changed = false;
    return b;
}

void NodeEditor::on_tree_change()
{
    _tree_has_changed = true;
}

void NodeEditor::open_menu()
{
    _should_open_menu = true;
}

bool NodeEditor::wants_to_delete_selection() const
{
    return ImGui::IsKeyReleased(GLFW_KEY_DELETE) ||
           ImGui::IsKeyReleased(GLFW_KEY_BACKSPACE) ||
           ImGui::IsKeyReleased(GLFW_KEY_X);
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

bool NodeEditor::handle_link_creation()
{
    int from_pin_id, to_pin_id;
    if (ImNodes::IsLinkCreated(&from_pin_id, &to_pin_id)) {
        _tree.delete_link_going_to(PinId{to_pin_id});
        _tree.add_link(Link{.from_pin_id = PinId{from_pin_id},
                            .to_pin_id   = PinId{to_pin_id}});
        return true;
    }
    return false;
}

bool NodeEditor::handle_link_deletion()
{
    bool has_deleted_some = false;
    {
        int link_id;
        if (ImNodes::IsLinkDestroyed(&link_id)) {
            _tree.delete_link(LinkId{link_id});
            has_deleted_some = true;
        }
    }

    {
        const int num_selected = ImNodes::NumSelectedLinks();
        if (num_selected > 0 && wants_to_delete_selection()) {
            has_deleted_some = true;
            static std::vector<int> selected_links;
            selected_links.resize(static_cast<size_t>(num_selected));
            ImNodes::GetSelectedLinks(selected_links.data());
            for (const int link_id : selected_links) {
                _tree.delete_link(LinkId{link_id});
            }
        }
    }
    return has_deleted_some;
}

bool NodeEditor::handle_node_deletion()
{
    const int num_selected = ImNodes::NumSelectedNodes();
    if (num_selected > 0 && wants_to_delete_selection()) {
        static std::vector<int> selected_nodes;
        selected_nodes.resize(static_cast<size_t>(num_selected));
        ImNodes::GetSelectedNodes(selected_nodes.data());
        for (const int node_id : selected_nodes) {
            _tree.delete_node(NodeId{node_id});
        }
        ImNodes::ClearNodeSelection();
        return true;
    }
    return false;
}

void NodeEditor::imgui_window()
{
    bool node_tree_has_changed = false;
    ImGui::Begin("is0");
    ImNodes::BeginNodeEditor();
    {
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle) || _should_open_menu) {
            _should_open_menu = false;
            ImGui::OpenPopup("_node_templates_list");
            _factory.clear_filter();
            _next_node_position = ImGui::GetMousePosOnOpeningCurrentPopup();
        }
        if (ImGui::BeginPopup("_node_templates_list")) {
            node_tree_has_changed |= imgui_nodes_menu();
            ImGui::EndPopup();
        }
        for (auto& node : _tree.nodes) {
            show_node(node, [&]() { on_tree_change(); });
        }
        for (const auto& link : _tree.links) {
            show_link(link);
        }
    }
    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    ImNodes::EndNodeEditor();
    node_tree_has_changed |= handle_link_creation();
    node_tree_has_changed |= handle_link_deletion();
    node_tree_has_changed |= handle_node_deletion();
    ImGui::End();
    if (node_tree_has_changed) {
        on_tree_change();
    }
}

bool NodeEditor::imgui_nodes_menu()
{
    const std::optional<Node> node = _factory.imgui();
    if (node.has_value()) {
        _tree.add_node(*node);
        ImNodes::SetNodeScreenSpacePos(node->id, _next_node_position);
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
            node.parameter_list = Cool::ParameterU::update_parameters(node_template->parameters, node.parameter_list);
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
    on_tree_change();
}

void NodeEditor::generate_a_random_scene()
{
    _tree = random_node_tree(_factory);
}