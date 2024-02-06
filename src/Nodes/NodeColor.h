#pragma once
#include <Cool/Dependencies/SharedVariableDefinition.h>
#include <Cool/StrongTypes/Color.h>
#include "FunctionSignature.h"
#include "PrimitiveType.h"

namespace Lab {

namespace NodeColor {
#include "generated_node_kind/node_colors_declaration.inl"
} // namespace NodeColor

enum class NodeKind {
#include "generated_node_kind/node_kind_enum.inl"
};

auto compute_function_color(FunctionSignature) -> Cool::Color;
auto compute_value_input_color(Cool::AnySharedVariableDefinition const&) -> Cool::Color;
auto compute_primitive_type_color(PrimitiveType) -> Cool::Color;
auto node_kind_color(NodeKind) -> Cool::Color;
auto imgui_node_kind_dropdown(const char* label, NodeKind*) -> bool;

} // namespace Lab