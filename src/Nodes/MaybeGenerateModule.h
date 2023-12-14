#pragma once
#include <Nodes/NodeDefinition.h>
#include <optional>
#include "Cool/Nodes/NodeId.h"

namespace Lab {

/// Called for each node.
/// Returns a string iff a new module has been generated and gen_desired_function() should read the result of the module fom a texture (the name of the texture is the name returned by the function).
using MaybeGenerateModule = std::function<std::optional<std::string>(Cool::NodeId const&, NodeDefinition const&)>;

} // namespace Lab