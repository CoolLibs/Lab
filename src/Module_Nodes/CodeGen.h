#pragma once
#include <Cool/Nodes/NodeId.h>
#include "CodeGenContext.h"
#include "FunctionSignature.h"

namespace Lab {

auto gen_desired_function(
    FunctionSignature   desired_signature,
    Cool::NodeId const& id,
    CodeGenContext&     context
) -> ExpectedFunctionName;

auto make_valid_output_index_name(Cool::OutputPin const& pin)
    -> std::string;

} // namespace Lab