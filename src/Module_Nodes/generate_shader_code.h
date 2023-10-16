#pragma once

#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/Graph.h>
#include "Cool/Dependencies/ValueProvider.h"
#include "NodeDefinition.h"

namespace Lab {

auto generate_shader_code(
    Cool::NodeId const& main_node_id,
    Cool::Graph const&,
    Cool::GetNodeDefinition_Ref<NodeDefinition>,
    Cool::ValueProvider
) -> tl::expected<std::string, std::string>;

auto valid_property_name(
    std::string const& name,
    reg::AnyId const&  property_default_variable_id
) -> std::string;

} // namespace Lab