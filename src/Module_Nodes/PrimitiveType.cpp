#include "PrimitiveType.h"

namespace Lab {

auto raw_glsl_type_as_string(PrimitiveType type) -> std::string
{
    switch (type)
    {
#include "generated/raw_glsl_type_as_string_cases.inl"
    default:
        return "ERROR in raw_glsl_type_as_string(): unknown type";
    }
}

auto glsl_type_as_string(PrimitiveType type) -> std::string
{
    switch (type)
    {
#include "generated/glsl_type_as_string_cases.inl"
    default:
        return "ERROR in glsl_type_as_string(): unknown type";
    }
}

auto cpp_type_as_string(PrimitiveType type) -> std::string
{
    switch (type)
    {
#include "generated/cpp_type_as_string_cases.inl"
    default:
        return "ERROR in cpp_type_as_string(): unknown type";
    }
}

auto can_convert(PrimitiveType from, PrimitiveType to) -> bool // NOLINT(readability-function-cognitive-complexity)
{
    if (from == to)
        return true;

    {
#include "generated/can_convert.inl"
    }
    return false;
}

auto is_color_type(PrimitiveType type) -> bool
{
    return
#include "generated/is_color_type.inl"
        ;
}

auto is_greyscale_type(PrimitiveType type) -> bool
{
    return
#include "generated/is_greyscale_type.inl"
        ;
}

auto is_color_or_greyscale_type(PrimitiveType type) -> bool
{
    return is_greyscale_type(type) || is_color_type(type);
}

auto has_an_alpha_channel(PrimitiveType type) -> bool
{
    switch (type)
    {
#include "generated/has_an_alpha_channel.inl"
    default:
        return false;
    }
}

auto has_straight_alpha_channel(PrimitiveType type) -> bool
{
    switch (type)
    {
#include "generated/has_straight_alpha_channel.inl"
    default:
        return false;
    }
}

auto with_straight_alpha(PrimitiveType type) -> PrimitiveType
{
    switch (type)
    {
#include "generated/with_straight_alpha.inl"
    default:
    {
        assert(false && "This type is not a color or greyscale, or already has an alpha channel.");
        return PrimitiveType::LinearRGB_StraightA;
    }
    }
}

auto with_straight_alpha_if_has_no_alpha(PrimitiveType type) -> PrimitiveType
{
    switch (type)
    {
#include "generated/with_straight_alpha.inl"
    default:
        return type;
    }
}

} // namespace Lab