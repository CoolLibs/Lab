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

static constexpr auto Colorizer = FunctionSignature{
    .from = PrimitiveType::Float{},
    .to   = PrimitiveType::Color{},
};

} // namespace Signature

} // namespace Lab