#pragma once
#include <Cool/Dependencies/AnyInput.h>
#include "PrimitiveType.h"

namespace Lab {

auto input_to_primitive_type(Cool::AnyInput const&) -> std::optional<PrimitiveType>;

}