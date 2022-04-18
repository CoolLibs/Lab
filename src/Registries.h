#pragma once

#include <Cool/Camera/Camera.h>
#include <reg/reg.hpp>

/// All the registries are owned by the top-level app, in order to simplify the changes through commands, the history, etc.

namespace Lab {

using Registries = reg::Registries<Cool::Camera,
                                   float,
                                   glm::vec3>;

} // namespace Lab
