#pragma once

#include <Cool/Variables/Variable.h>
#include <reg/reg.hpp>

namespace Lab {

template<typename T>
using VariableId = reg::Id<Cool::Variable<T>>;

}