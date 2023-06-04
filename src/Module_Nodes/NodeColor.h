#pragma once
#include <Cool/StrongTypes/Color.h>
#include "FunctionSignature.h"

namespace Lab {

namespace NodeColor {
#include "generated_node_kind/node_colors_declaration.inl"
} // namespace NodeColor

auto compute_node_color(FunctionSignature) -> Cool::Color;

} // namespace Lab