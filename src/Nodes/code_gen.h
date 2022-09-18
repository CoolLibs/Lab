#pragma once

#include "FunctionSignature.h"
#include "GlslCode.h"
#include "Node.h"
#include "NodeDefinition.h"
#include "NodeID.h"
#include "NodeLibrary.h"

namespace Lab {

auto gen_base_function(
    NodeDefinition const&,
    NodeID const&
) -> GlslCode;

auto gen_desired_function(
    Node const&,
    NodeID const&,
    FunctionSignature const& desired_signature,
    std::string_view         name,
    NodeLibrary&             library
) -> std::optional<GlslCode>;

} // namespace Lab