#pragma once

#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <tl/expected.hpp>
#include "Graph.h"
#include "NodeDefinition.h"

namespace Lab {

auto generate_shader_code(
    Graph const&,
    Cool::GetNodeDefinition_Ref<NodeDefinition> const&,
    Cool::NodeId const& main_node_id,
    Cool::InputProvider_Ref
) -> tl::expected<std::string, std::string>;

}