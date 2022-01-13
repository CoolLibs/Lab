#pragma once
#include <Cool/Parameter/Parameter.h>

struct RenderEffect_Fresnel {
    Cool::Parameter::Float strenght{{.name          = "Strenght",
                                     .default_value = 0.9f,
                                     .min_value     = 0.0f,
                                     .max_value     = 10.f}};
    Cool::Parameter::Color col;
    bool                   isActive = true;
};

std::string fresnelParameters(const RenderEffect_Fresnel& fresnel);
std::string fresnelAdd();