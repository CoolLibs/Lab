#pragma once

#include "PrimitiveType.h"

namespace Lab {

struct FunctionSignature {
    AnyPrimitiveType from;
    AnyPrimitiveType to;

    friend auto operator==(const FunctionSignature&, const FunctionSignature&) -> bool = default;
};

namespace Signature {

static constexpr auto Image = FunctionSignature{
    .from = PrimitiveType::UV{},
    .to   = PrimitiveType::Color{},
};

static constexpr auto FloatField = FunctionSignature{
    .from = PrimitiveType::UV{},
    .to   = PrimitiveType::Float{},
};

} // namespace Signature

} // namespace Lab