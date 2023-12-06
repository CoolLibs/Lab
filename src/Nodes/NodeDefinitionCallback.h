#pragma once
#include <Nodes/NodeDefinition.h>
#include "Cool/Nodes/NodeId.h"

namespace Lab {

/// Called for each node.
/// Returns true iff the node has been handled and gen_desired_function() should exit.
using NodeDefinitionCallback = std::function<bool(Cool::NodeId const&, NodeDefinition const&)>;

} // namespace Lab