#include "DirtyFlags.h"

namespace Lab {

auto DirtyFlags::primary(bool always_requires_shader_code_generation) const -> Cool::DirtyFlag const&
{
    return always_requires_shader_code_generation ? regenerate_code : rerender;
}

auto DirtyFlags::secondary() const -> Cool::DirtyFlag const&
{
    return regenerate_code; // At the moment only used by Gradient variable when we detect that the number of marks has changed. See `set_value()` of Command_SetVariable.h
}

} // namespace Lab