#pragma once
#include <Module/ModuleDependencies.h>
#include "Cool/WebGPU/BindGroupLayout.hpp"

namespace Lab {

auto make_system_bind_group_layout(ModuleDependencies const&) -> Cool::BindGroupLayout;

}