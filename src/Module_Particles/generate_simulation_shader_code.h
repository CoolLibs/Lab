#pragma once
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodesGraph.h>
#include "Nodes/NodeDefinition.h"

namespace Lab {

auto generate_simulation_shader_code(
    Cool::NodesGraph const&,
    Cool::NodeId const& root_node_id,
    Cool::NodeId&       id_of_node_storing_particles_count,
    Cool::GetNodeDefinition_Ref<NodeDefinition>,
    int dimension
) -> tl::expected<std::string, std::string>;

} // namespace Lab
