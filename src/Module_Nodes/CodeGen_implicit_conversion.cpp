#include "CodeGen_implicit_conversion.h"

namespace Lab {

auto gen_implicit_conversion(PrimitiveType from, PrimitiveType to, CodeGenContext& context) // NOLINT(readability-function-cognitive-complexity)
    -> std::optional<std::string>
{
    if (from == to)
        return "";

    if (from == PrimitiveType::Float && to == PrimitiveType::Int)
    {
        return context.push_function({
            .name           = "CoolLab_float_to_int",
            .implementation = R"STR(
int CoolLab_float_to_int/*coollabdef*/(float x)
{
    return int(floor(x));
}
)STR",
        });
    }
    if (from == PrimitiveType::Int && to == PrimitiveType::Float)
    {
        return context.push_function({
            .name           = "CoolLab_int_to_float",
            .implementation = R"STR(
float CoolLab_int_to_float/*coollabdef*/(int x)
{
    return float(x);
}
)STR",
        });
    }

    if (from == PrimitiveType::Angle && to == PrimitiveType::Direction2D)
    {
        return context.push_function({
            .name           = "CoolLab_angle_to_direction2D",
            .implementation = R"STR(
vec2 CoolLab_angle_to_direction2D/*coollabdef*/(float angle)
{
    return vec2(cos(angle), sin(angle));
}
)STR",
        });
    }
    if (from == PrimitiveType::Direction2D && to == PrimitiveType::Angle)
    {
        return context.push_function({
            .name           = "CoolLab_direction2D_to_angle",
            .implementation = R"STR(
float CoolLab_direction2D_to_angle/*coollabdef*/(vec2 dir)
{
    return dir.x != 0.f
                ? atan(dir.y, dir.x)
                : dir.y > 0.
                    ? PI / 2.
                    : -PI / 2.;
}
)STR",
        });
    }

    // No need to do anything for these conversions, the difference is purely semantic.
    if (from == PrimitiveType::Float && to == PrimitiveType::SignedDistance)
        return "";
    if (from == PrimitiveType::SignedDistance && to == PrimitiveType::Float)
        return "";

    // No need to do anything for these conversions, the difference is purely semantic.
    if (from == PrimitiveType::UV && to == PrimitiveType::Vec2)
        return "";
    if (from == PrimitiveType::Vec2 && to == PrimitiveType::UV)
        return "";

#include "generated/implicit_color_conversions.inl"

    return std::nullopt;
}

auto gen_implicit_conversions(FunctionSignature current, FunctionSignature desired, CodeGenContext& context)
    -> ImplicitConversions
{
    return ImplicitConversions{
        .input  = gen_implicit_conversion(current.from, desired.from, context),
        .output = gen_implicit_conversion(current.to, desired.to, context),
    };
}

} // namespace Lab
