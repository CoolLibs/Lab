#pragma once

#include <reg/reg.hpp>
#include "AllVariableTypes.h"

/// All the registries are owned by the top-level app, in order to simplify the changes through commands, the history, etc.

namespace Lab {

using VariableRegistries = AllVariableTypes::to<reg::Registries>;

} // namespace Lab
