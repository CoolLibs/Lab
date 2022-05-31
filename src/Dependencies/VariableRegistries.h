#pragma once

#include <reg/reg.hpp>
#include "AllVariableTypes.h"

namespace Lab {

/// All the registries are owned by the top-level app, in order to simplify the changes through commands, the history, etc.
using VariableRegistries = AllVariableTypes::to<reg::Registries>;

} // namespace Lab
