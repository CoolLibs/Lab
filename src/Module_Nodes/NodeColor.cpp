#include "NodeColor.h"

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

} // namespace Lab