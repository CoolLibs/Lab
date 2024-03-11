#include "CodeGen_implicit_conversion.h"
#include "Function.h"
#include "PrimitiveType.h"

namespace Lab {

#include "generated/def_implicit_color_conversions.inl"
#include "generated/def_implicit_conversions.inl"

auto gen_implicit_conversion(PrimitiveType from, PrimitiveType to, CodeGenContext& context) // NOLINT(readability-function-cognitive-complexity)
    -> std::optional<std::string>
{
    if (from == to)
        return "";

#include "generated/implicit_conversions.inl"
//
#include "generated/implicit_color_conversions.inl"

    return std::nullopt;
}

auto gen_implicit_conversions(FunctionSignature current, FunctionSignature desired, CodeGenContext& context)
    -> ImplicitConversions
{
    return ImplicitConversions{
        .input  = gen_implicit_conversion(desired.from, current.from, context),
        .output = gen_implicit_conversion(current.to, desired.to, context),
    };
}

} // namespace Lab
