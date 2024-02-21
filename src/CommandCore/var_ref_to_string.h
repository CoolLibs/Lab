#pragma once
#include "Cool/Dependencies/SharedVariable.h"

namespace Lab {

template<typename T>
auto to_string(Cool::SharedVariableStrongRef<T> const& var_ref) -> std::string
{
    return fmt::format("{} (id {})", var_ref.variable->name(), var_ref.id());
}

} // namespace Lab