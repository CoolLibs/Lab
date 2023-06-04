#pragma once
#include <Cool/StrongTypes/Color.h>
#include "FunctionSignature.h"

namespace Lab {

auto compute_node_color(FunctionSignature) -> Cool::Color;

} // namespace Lab