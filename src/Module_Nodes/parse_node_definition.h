#pragma once
#include "Module_Nodes/NodeDefinition.h"
#include "NodeDefinition.h"

namespace Lab {

auto parse_node_definition(std::string const& name, std::string text) -> tl::expected<NodeDefinition_Data, std::string>;

} // namespace Lab