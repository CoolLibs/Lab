#pragma once

#include "Node.h"
#include "NodeDefinition.h"

namespace Lab {

struct NodesConfig {
    using NodeT           = Node;
    using NodeDefinitionT = NodeDefinition;

    static auto name(Node const& node) -> std::string;
    static auto make_node(NodeDefinition const& def) -> Node;
};

} // namespace Lab
