#pragma once

#include "Dependencies/VariableRegistries.h"

namespace Lab {

struct MakeReversibleCommandContext_Ref {
    std::reference_wrapper<VariableRegistries> registries;
};

} // namespace Lab
