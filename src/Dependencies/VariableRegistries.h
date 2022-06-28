#pragma once

#include <Cool/Variables/Variable.h>
#include <reg/cereal.hpp>
#include <reg/reg.hpp>
#include "generated_variables/all_variable_includes.inl"

namespace Lab {

/// All the registries are owned by the top-level app, in order to simplify the changes through commands, the history, etc.
#include "generated_variables/VariableRegistries.inl"

} // namespace Lab
