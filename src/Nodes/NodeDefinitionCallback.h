#pragma once
#include <Nodes/NodeDefinition.h>
#include <optional>
#include "Cool/Nodes/NodeId.h"

namespace Lab {

/// Called for each node.
/// Returns true if the node has been handled and gen_desired_function() should exit.
using NodeDefinitionCallback = std::function<std::optional<std::string>(Cool::NodeId const&, NodeDefinition const&)>;

} // namespace Lab