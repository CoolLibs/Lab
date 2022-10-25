#pragma once

#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodeId.h>
#include "FunctionSignature.h"
#include "GlslCode.h"
#include "Node.h"
#include "NodeDefinition.h"

namespace Lab {

auto gen_base_function(
    NodeDefinition const&,
    Cool::NodeId const&
) -> GlslCode;

auto gen_desired_function(
    Node const&,
    Cool::NodeId const&,
    FunctionSignature const&                    desired_signature,
    std::string_view                            name,
    std::string_view                            input_function_name,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition
) -> std::optional<GlslCode>;

} // namespace Lab