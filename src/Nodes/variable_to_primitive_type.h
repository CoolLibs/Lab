#pragma once
#include <Cool/Dependencies/AnySharedVariable.h>
#include "PrimitiveType.h"

namespace Lab {

auto variable_to_primitive_type(Cool::AnySharedVariable const&) -> std::optional<PrimitiveType>;

}