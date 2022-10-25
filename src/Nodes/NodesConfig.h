#pragma once

#include "Node.h"
#include "NodeDefinition.h"

namespace Lab {

struct NodesConfig {
    using NodeT           = Node;
    using NodeDefinitionT = NodeDefinition;
    static auto name(Node const&) -> const char* { return "Some Node"; }
    static auto make_node(NodeDefinition const&) -> Node
    {
        // TODO(JF)
        return Node{};
    }
};

} // namespace Lab
