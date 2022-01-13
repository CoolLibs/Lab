#pragma once
#include <Cool/Parameter/Parameter.h>

struct RenderEffect_Reflection {
    Cool::Parameter::Float strenght{{.name          = "IOR",
                                     .default_value = 0.7f,
                                     .min_value     = 0.01f,
                                     .max_value     = 2.5f}};
};

std::string reflectionParameters(const RenderEffect_Reflection& reflection);