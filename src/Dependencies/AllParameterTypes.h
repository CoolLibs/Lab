#pragma once

#include <Cool/Camera/Camera.h>
#include <cpp-typelist/typelist.hpp>

using AllParameterTypes = tl::type_list<Cool::Camera,
                                        int,
                                        float,
                                        glm::vec2,
                                        glm::vec3>;