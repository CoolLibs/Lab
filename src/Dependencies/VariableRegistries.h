#pragma once

#include <Cool/Variables/Variable.h>
#include <Cool/Variables/Variables.h>
#include <reg/reg.hpp>
#include "generated/all_variable_includes.inl"

namespace Lab {

/// All the registries are owned by the top-level app, in order to simplify the changes through commands, the history, etc.
#include "generated/VariableRegistries.inl"

} // namespace Lab
