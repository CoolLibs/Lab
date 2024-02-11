#pragma once
#include "Nodes/NodesAndLinksGroup.h"

namespace Lab {

auto nodes_and_links_group_to_string(NodesAndLinksGroup const&) -> std::string;
auto nodes_and_links_group_from_string(std::string const&) -> NodesAndLinksGroup;

} // namespace Lab