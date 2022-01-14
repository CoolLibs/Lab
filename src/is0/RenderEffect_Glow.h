#pragma once
#include <Cool/Parameter/Parameter.h>

struct RenderEffect_Glow {
    Cool::Parameter::Float strenght{{.name          = "Strenght",
                                     .default_value = 0.005f,
                                     .min_value     = 0.0f,
                                     .max_value     = 0.05f}};
    Cool::Parameter::Color col;
    bool                   is_active = true;
};

std::string code_gen_glow_parameters(const RenderEffect_Glow& glow);
std::string code_gen_glow();