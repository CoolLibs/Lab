#pragma once
#include <Cool/Parameter/Parameter.h>

struct RenderEffect_Fresnel {
    Cool::Parameter::Float strenght{{.name          = "Strenght",
                                     .default_value = 0.9f,
                                     .min_value     = 0.0f,
                                     .max_value     = 10.f}};
    Cool::Parameter::Color col;
    bool                   is_active = true;
};

std::string fresnel_parameters(const RenderEffect_Fresnel& fresnel);
std::string fresnelAdd();