#pragma once

#include "Link.h"
#include "Node.h"

namespace NodeEditorU {

/**
 * @brief Assumes that pin is an input pin and returns the node that is connected to it (or nullptr if there is none)
 */
const Node* find_input_node(const Pin& pin, const std::vector<Node>& nodes, const std::vector<Link>& links);

const Node& find_node_with_output_pin(ed::PinId pin_id, const std::vector<Node>& nodes);

bool has_no_successor(const Node& node, const std::vector<Link>& links);

} // namespace NodeEditorU