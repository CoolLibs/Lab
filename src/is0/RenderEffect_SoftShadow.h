#pragma once

#include <Cool/Parameter/Parameter.h>

struct RenderEffect_Shadow {
    Cool::Parameter::Float strenght{{.name          = "Strenght",
                                     .default_value = 8.0,
                                     .min_value     = 2.0,
                                     .max_value     = 124.0}};
    bool                   isActive = true;
};

std::string SoftShadowParameter(const RenderEffect_Shadow& shadow);
std::string SoftShadowfct();
std::string SoftShadowAdd();
