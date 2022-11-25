#pragma once

#include "PrimitiveType.h"

namespace Lab {

struct FunctionSignature {
    PrimitiveType from;
    PrimitiveType to;
    size_t        arity;

    friend auto operator==(const FunctionSignature&, const FunctionSignature&) -> bool = default;
};

inline auto to_string(FunctionSignature signature) -> std::string
{
    return fmt::format("{}to{}Arity{}", cpp_type_as_string(signature.from), cpp_type_as_string(signature.to), signature.arity);
}

namespace Signature {

static constexpr auto Image = FunctionSignature{
    .from  = PrimitiveType::UV,
    .to    = PrimitiveType::Color,
    .arity = 1,
};

static constexpr auto FloatField = FunctionSignature{
    .from  = PrimitiveType::UV,
    .to    = PrimitiveType::Float,
    .arity = 1,
};

static constexpr auto Colorizer = FunctionSignature{
    .from  = PrimitiveType::Float,
    .to    = PrimitiveType::Color,
    .arity = 1,
};

static constexpr auto BlendMode = FunctionSignature{
    .from  = PrimitiveType::Color, // TODO(JF) Use RGBA
    .to    = PrimitiveType::Color, // TODO(JF) Use RGBA
    .arity = 2,
};

} // namespace Signature

} // namespace Lab