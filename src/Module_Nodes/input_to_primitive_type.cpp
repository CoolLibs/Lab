#include "input_to_primitive_type.h"

namespace Lab {

auto input_to_primitive_type(Cool::AnyInput const& input) -> std::optional<PrimitiveType>
{
#include "generated/input_to_primitive_type.inl"
    return std::nullopt;
}

} // namespace Lab