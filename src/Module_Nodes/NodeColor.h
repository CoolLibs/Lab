#pragma once
#include <Cool/Dependencies/InputDefinition.h>
#include <Cool/StrongTypes/Color.h>
#include <Module_Nodes/PrimitiveType.h>
#include "FunctionSignature.h"

namespace Lab {

namespace NodeColor {
#include "generated_node_kind/node_colors_declaration.inl"
} // namespace NodeColor

auto compute_function_color(FunctionSignature) -> Cool::Color;
auto compute_value_input_color(Cool::AnyInputDefinition const&) -> Cool::Color;
auto compute_primitive_type_color(PrimitiveType) -> Cool::Color;

} // namespace Lab