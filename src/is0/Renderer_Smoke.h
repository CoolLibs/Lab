#pragma once

#include <Cool/Parameter/ParameterList.h>
#include "NodeTemplate.h"
#include "NodeTree.h"

namespace CodeGen {

struct DefineVariables {
    float ABSORPTION_COEFFICIENT = 0.5;

    // Reduce the following values to enhance performance
    float MAX_VOLUME_MARCH_STEPS = 50;
    float MARCH_MULTIPLIER       = 1.8;
};

struct SmokeProperties {
    float precis           = 0.5;
    float opaqueVisibility = 1.0f;
};

std::string IntersVolumCodeGen(const SmokeProperties& p);
std::string GetLightVisCodeGen();
std::string Render(const SmokeProperties& p, const DefineVariables& d);
std::string SmokeRenderer(const SmokeProperties& p, const DefineVariables& d);

} // namespace CodeGen
