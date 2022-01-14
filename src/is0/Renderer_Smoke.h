#pragma once

#include <Cool/Parameter/ParameterList.h>
#include "NodeTemplate.h"
#include "NodeTree.h"

namespace CodeGen {

struct RenderEffect_Smoke {
    Cool::Parameter::Float ABSORPTION_COEFFICIENT{{.name          = "Absorption Coefficient",
                                                   .default_value = 2.5,
                                                   .min_value     = 0.01,
                                                   .max_value     = 5.}};

    // Reduce the following values to enhance performance

    Cool::Parameter::Float MARCH_MULTIPLIER{{.name          = "March Multiplier",
                                             .default_value = 0.5,
                                             .min_value     = 0.01,
                                             .max_value     = 3.}};

    Cool::Parameter::Float MAX_VOLUME_MARCH_STEPS{{.name          = "Max Volume March Steps",
                                                   .default_value = 50.0,
                                                   .min_value     = 20.0,
                                                   .max_value     = 75.0}};

    bool is_active = true;
};

std::string SmokeParameters(const RenderEffect_Smoke& parameters);
std::string IntersVolumCodeGen();
std::string GetLightVisCodeGen();
std::string Render();
std::string SmokeRenderer(const RenderEffect_Smoke& p);
std::string addSmoke(const RenderEffect_Smoke& p);

} // namespace CodeGen
