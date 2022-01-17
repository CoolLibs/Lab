#pragma once

#include <Cool/Parameter/Parameter.h>

struct RenderEffect_SSAO {
    Cool::Parameter::Float size{{.name          = "Size",
                                 .default_value = 0.1,
                                 .min_value     = 0.001,
                                 .max_value     = 2.0}};
    bool                   is_active = true;
};

std::string code_gen_SSAO_parameter(const RenderEffect_SSAO& SSAO);
std::string code_gen_SSAO_fct();
std::string code_gen_SSAO();

bool SSAO_imgui(RenderEffect_SSAO& SSAO);