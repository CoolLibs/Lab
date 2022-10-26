#pragma once

#include "NodesConfig.h"

namespace Lab {

auto NodesConfig::name(Node const& node) -> std::string
{
    return node.definition_name();
}

auto NodesConfig::make_node(NodeDefinition const& def) -> Node
{
    return Node{def.name()};
}

} // namespace Lab
