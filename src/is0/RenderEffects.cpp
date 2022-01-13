#include "RenderEffects.h"

std::string effectsParameters(const RenderEffects& effects)
{
    return fresnelParameters(effects._fresnel) +
           glowParameters(effects._glow) +
           reflectionParameters(effects._reflection);
};

std::string addParameters(const RenderEffects& effects)
{
    std::string code = "";
    if (effects._fresnel.isActive)
        code += fresnelAdd();
    if (effects._reflection.isActive)
        code += reflectionAdd();
    code += "}";
    if (effects._glow.isActive)
        code += glowAdd();
    return code;
};