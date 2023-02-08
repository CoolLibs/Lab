#pragma once
#include "Module_Nodes/NodeDefinition.h"
#include "NodeDefinition.h"

namespace Lab {

auto parse_node_definition(std::filesystem::path filepath, std::string const& text)
    -> tl::expected<NodeDefinition, std::string>;

} // namespace Lab