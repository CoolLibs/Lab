#pragma once

#include <Cool/Parameter/Parameter.h>

struct RenderEffect_InigoAO {
    Cool::Parameter::Float size{{.name          = "Size",
                                 .default_value = 0.1,
                                 .min_value     = 0.001,
                                 .max_value     = 2.0}};
    bool                   is_active = true;
};

std::string code_gen_InigoAO_parameter(const RenderEffect_InigoAO& InigoAO);
std::string code_gen_InigoAO_fct();
std::string code_gen_InigoAO();

bool InigoAO_imgui(RenderEffect_InigoAO& InigoAO);
