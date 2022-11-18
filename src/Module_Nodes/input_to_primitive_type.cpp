#include "input_to_primitive_type.h"

namespace Lab {

auto input_to_primitive_type(Cool::AnyInput const& input) -> std::optional<AnyPrimitiveType>
{
    if (std::holds_alternative<Cool::Input<float>>(input))
        return PrimitiveType::Float{};

    return std::nullopt;
}

} // namespace Lab