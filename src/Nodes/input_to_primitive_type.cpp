#include "input_to_primitive_type.h"
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"

namespace Lab {

auto input_to_primitive_type(Cool::AnyInput const& input) -> std::optional<PrimitiveType>
{
    {
#include "generated/input_to_primitive_type.inl"
    } // Braces are here to silence a "misleading indent" warning

    return std::nullopt;
}

} // namespace Lab