#pragma once
#include <Cool/Nodes/NodeId.h>
#include "CodeGenContext_Ref.h"
#include "FunctionSignature.h"

namespace Lab {

auto gen_desired_function(
    FunctionSignature   desired_signature,
    Cool::NodeId const& id,
    CodeGenContext_Ref  context
) -> tl::expected<Function, std::string>;

auto make_valid_output_index_name(Cool::OutputPin const& pin)
    -> std::string;

} // namespace Lab