#include "PrimitiveType.h"

namespace Lab {

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

} // namespace Lab