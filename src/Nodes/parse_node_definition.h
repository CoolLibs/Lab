#pragma once
#include "NodeDefinition.h"

namespace Lab {

auto parse_node_definition(std::filesystem::path filepath, std::string text)
    -> tl::expected<NodeDefinition, std::string>;

} // namespace Lab