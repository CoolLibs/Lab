#include "RenderEffects.h"

std::string effectsParameters(const RenderEffects& effects)
{
    return fresnelParameters(effects._fresnel) +
           glowParameters(effects._glow);
};