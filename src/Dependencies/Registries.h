#pragma once

#include <reg/reg.hpp>
#include "AllParameterTypes.h"

/// All the registries are owned by the top-level app, in order to simplify the changes through commands, the history, etc.

namespace Lab {

using Registries = AllParameterTypes::to<reg::Registries>;

} // namespace Lab
