#pragma once
#include "Function.h"
#include "FunctionSignature.h"

namespace Lab {

auto gen_default_function(FunctionSignature, AlreadyGeneratedFunctions&)
    -> tl::expected<Function, std::string>;

} // namespace Lab
