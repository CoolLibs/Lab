#pragma once
#include "CodeGenContext.h"
#include "FunctionSignature.h"
#include "Node.h"

namespace Lab {

// Returns the function's body, or an error message in the unexpected case.
auto gen_desired_function_implementation(
    FunctionSignature current,
    FunctionSignature desired,
    std::string_view  base_function_name,
    Node const&,
    CodeGenContext&
) -> tl::expected<std::string, std::string>;

} // namespace Lab
