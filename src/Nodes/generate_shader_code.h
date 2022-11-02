#pragma once

#include "Graph.h"
#include "NodesLibrary.h"

namespace Lab {

auto generate_shader_code(
    Graph const&,
    NodesLibrary const&,
    Cool::NodeId const& main_node_id
) -> std::string;

}