#pragma once

#include <Cool/Parameter/Parameter.h>

struct RenderEffect_Shadow {
    Cool::Parameter::Float strenght{{.name          = "Strenght",
                                     .default_value = 8.0,
                                     .min_value     = 2.0,
                                     .max_value     = 124.0}};
    bool                   is_active = true;
};

std::string code_gen_softShadow_parameter(const RenderEffect_Shadow& shadow);
std::string code_gen_softShadow_fct();
std::string code_gen_softShadow();
bool        softShadow_imgui(RenderEffect_Shadow& shadow);