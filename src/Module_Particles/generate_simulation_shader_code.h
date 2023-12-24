#pragma once
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodesGraph.h>
#include "Cool/Dependencies/InputProvider_Ref.h"
#include "Nodes/NodeDefinition.h"

namespace Lab {

auto generate_simulation_shader_code(
    Cool::NodesGraph const&,
    Cool::NodeId const& root_node_id,
    Cool::NodeId&       initializer_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition>,
    Cool::InputProvider_Ref,
    int dimension
) -> tl::expected<std::string, std::string>;

auto valid_property_name(
    std::string const& name,
    reg::AnyId const&  property_default_variable_id
) -> std::string;

} // namespace Lab
