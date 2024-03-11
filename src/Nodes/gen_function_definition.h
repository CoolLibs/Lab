#pragma once
#include "FunctionSignatureAsString.h"

namespace Lab {

struct Params_gen_function_definition {
    FunctionSignatureAsString signature_as_string{};
    std::string               unique_name{};
    std::string               before_function{};
    std::string               body{};
};

auto gen_function_definition(Params_gen_function_definition const& p) -> std::string;

} // namespace Lab