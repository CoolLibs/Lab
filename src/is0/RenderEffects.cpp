#include "RenderEffects.h"

std::string add_effects_parameters(const RenderEffects& effects)
{
    return fresnel_parameters(effects.fresnel) +
           glow_parameters(effects.glow) +
           reflection_parameters(effects.reflection);
};

std::string addParameters(const RenderEffects& effects)
{
    std::string code = "";
    if (effects.fresnel.is_active)
        code += fresnelAdd();
    if (effects.reflection.is_active)
        code += reflectionAdd();
    code += "}";
    if (effects.glow.is_active)
        code += glowAdd();
    return code;
};