#include "NodeEditorU.h"

namespace NodeEditorU {

const Node* find_input_node(const Pin& pin, const std::vector<Node>& nodes, const std::vector<Link>& links)
{
    assert(pin.kind() == PinKind::Input);
    const auto link_it = std::ranges::find_if(links, [&](const Link& link) {
        return link.to_pin_id == pin.id();
    });
    if (link_it == links.end()) {
        return nullptr;
    }
    else {
        return &find_node_with_output_pin(link_it->from_pin_id, nodes);
    }
}

const Node& find_node_with_output_pin(PinId pin_id, const std::vector<Node>& nodes)
{
    return *std::ranges::find_if(nodes, [&](const Node& node) {
        return node.output_pin.id() == pin_id;
    });
}

bool has_no_successor(const Node& node, const std::vector<Link>& links)
{
    return std::ranges::find_if(links, [&](const Link& link) {
               return link.from_pin_id == node.output_pin.id();
           }) == links.end();
}

bool pins_are_from_different_nodes(PinId output_pin, PinId input_pin, const std::vector<Node>& nodes)
{
    const auto& node = find_node_with_output_pin(output_pin, nodes);
    return std::ranges::none_of(node.input_pins, [&](const Pin& pin) {
        return pin.id() == input_pin;
    });
}

} // namespace NodeEditorU