#pragma once
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodesGraph.h>
#include "Cool/Dependencies/InputProvider_Ref.h"
#include "Nodes/NodeDefinition.h"
#include "Nodes/NodeDefinitionCallback.h"

namespace Lab {

auto generate_compositing_shader_code(
    Cool::NodesGraph const&,
    Cool::NodeId const& root_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition>,
    Cool::InputProvider_Ref,
    NodeDefinitionCallback const&                    node_definition_callback,
    std::function<std::vector<std::string>()> const& get_textures_names
) -> tl::expected<std::string, std::string>;

auto valid_property_name(
    std::string const& name,
    reg::AnyId const&  property_default_variable_id
) -> std::string;

} // namespace Lab