#include "NodeColor.h"
#include <variant>
#include "Cool/StrongTypes/Angle.h"
#include "Cool/StrongTypes/ColorAndAlpha.h"
#include "Cool/StrongTypes/Direction2D.h"

namespace Lab {

namespace NodeColor {
#include "generated_node_kind/node_colors_definition.inl"
} // namespace NodeColor

auto compute_node_color(FunctionSignature signature) -> Cool::Color
{
#include "generated_node_kind/compute_node_color.inl"

    // Default color if everything else failed:
    return NodeColor::miscellaneous(); // NOLINT(*-misleading-indentation)
}

auto compute_value_input_color(Cool::AnyInputDefinition const& input_def) -> Cool::Color
{
    if (std::holds_alternative<Cool::InputDefinition<Cool::Color>>(input_def)
        || std::holds_alternative<Cool::InputDefinition<Cool::ColorAndAlpha>>(input_def))
        return NodeColor::image();

    if (std::holds_alternative<Cool::InputDefinition<float>>(input_def)
        || std::holds_alternative<Cool::InputDefinition<int>>(input_def)
        || std::holds_alternative<Cool::InputDefinition<Cool::Hue>>(input_def)
        || std::holds_alternative<Cool::InputDefinition<Cool::Angle>>(input_def)
        || std::holds_alternative<Cool::InputDefinition<Cool::Direction2D>>(input_def))
        return NodeColor::greyscale();

    return NodeColor::miscellaneous();
}

} // namespace Lab