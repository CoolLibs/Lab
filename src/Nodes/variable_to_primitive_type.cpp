#include "variable_to_primitive_type.h"
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"

namespace Lab {

auto variable_to_primitive_type(Cool::AnySharedVariable const& var) -> std::optional<PrimitiveType>
{
    {
#include "generated/variable_to_primitive_type.inl"
    } // Braces are here to silence a "misleading indent" warning

    return std::nullopt;
}

} // namespace Lab