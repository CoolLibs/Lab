#pragma once
#include "NodeDefinition.h"

namespace Lab {

auto parse_node_definition(std::filesystem::path const& filepath, std::string const& text)
    -> tl::expected<NodeDefinition, std::string>;

} // namespace Lab