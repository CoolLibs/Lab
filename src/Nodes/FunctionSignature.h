#pragma once

#include "PrimitiveType.h"

namespace Lab {

struct FunctionSignature {
    PrimitiveType from;
    PrimitiveType to;

    friend auto operator==(const FunctionSignature&, const FunctionSignature&) -> bool = default;
};

auto missing_functions(FunctionSignature current, FunctionSignature desired) -> std::vector<FunctionSignature>;

} // namespace Lab