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

} // namespace Lab