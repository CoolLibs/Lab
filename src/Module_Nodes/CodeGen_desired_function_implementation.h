#pragma once
#include "CodeGenContext_Ref.h"
#include "FunctionSignature.h"

namespace Lab {

struct FunctionImplementation {
    std::string before_function;
    std::string function_body;
};

auto gen_desired_function_implementation(
    FunctionSignature current,
    FunctionSignature desired,
    std::string_view  base_function_name,
    InputFunctionGenerator_Ref,
    DefaultFunctionGenerator_Ref
) -> tl::expected<FunctionImplementation, std::string>;

} // namespace Lab
