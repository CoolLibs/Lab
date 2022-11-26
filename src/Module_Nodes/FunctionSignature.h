#pragma once

#include "PrimitiveType.h"

namespace Lab {

struct FunctionSignature {
    PrimitiveType from;
    PrimitiveType to;
    size_t        arity{1};

    friend auto operator==(const FunctionSignature&, const FunctionSignature&) -> bool = default;
};

inline auto to_string(FunctionSignature signature) -> std::string
{
    return fmt::format("{}to{}Arity{}", cpp_type_as_string(signature.from), cpp_type_as_string(signature.to), signature.arity);
}

namespace Signature {

static constexpr auto ImageRGB = FunctionSignature{
    .from  = PrimitiveType::UV,
    .to    = PrimitiveType::RGB,
    .arity = 1,
};

static constexpr auto ImageRGBA = FunctionSignature{
    .from  = PrimitiveType::UV,
    .to    = PrimitiveType::RGBA,
    .arity = 1,
};

static constexpr auto FloatField = FunctionSignature{
    .from  = PrimitiveType::UV,
    .to    = PrimitiveType::Float,
    .arity = 1,
};

static constexpr auto ColorizerRGB = FunctionSignature{
    .from  = PrimitiveType::Float,
    .to    = PrimitiveType::RGB,
    .arity = 1,
};

static constexpr auto ColorizerRGBA = FunctionSignature{
    .from  = PrimitiveType::Float,
    .to    = PrimitiveType::RGBA,
    .arity = 1,
};

static constexpr auto BlendMode = FunctionSignature{
    .from  = PrimitiveType::RGBA,
    .to    = PrimitiveType::RGBA,
    .arity = 2,
};

} // namespace Signature

} // namespace Lab