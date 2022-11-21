#pragma once
#include "Function.h"
#include "FunctionSignature.h"
#include "PrimitiveType.h"

namespace Lab {

struct FunctionImplementation {
    std::string before_function;
    std::string function_body;
};

auto input_function_desired_signature(
    FunctionSignature current,
    FunctionSignature desired
) -> std::optional<FunctionSignature>;

auto gen_desired_function_implementation(
    FunctionSignature          current,
    FunctionSignature          desired,
    std::string_view           base_function_name,
    std::string_view           input_function_name,
    AlreadyGeneratedFunctions& already_generated_functions
) -> FunctionImplementation;

} // namespace Lab
