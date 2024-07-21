#pragma once
#include <Module/ModuleDependencies.h>
#include "Cool/WebGPU/BindGroup.hpp"
#include "Cool/WebGPU/BindGroupLayout.hpp"
#include "Dependencies/SystemValues.h"

namespace Lab {

auto make_system_bind_group(Cool::BindGroupLayout const&, SystemValues const&, ModuleDependencies const&) -> Cool::BindGroup;

}