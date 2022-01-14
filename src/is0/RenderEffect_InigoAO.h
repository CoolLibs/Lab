#pragma once

#include <Cool/Parameter/Parameter.h>

struct RenderEffect_InigoAO {
    Cool::Parameter::Float size{{.name          = "Size",
                                 .default_value = 0.1,
                                 .min_value     = 0.001,
                                 .max_value     = 2.0}};
    bool                   isActive = true;
};

std::string InigoAOParameter(const RenderEffect_InigoAO& InigoAO);
std::string InigoAOfct();
std::string InigoAOAdd();
