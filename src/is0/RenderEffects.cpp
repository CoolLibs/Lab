#include "RenderEffects.h"

std::string add_effects_parameters(const RenderEffects& effects)
{
    return code_gen_fresnel_parameters(effects.fresnel) +
           code_gen_glow_parameters(effects.glow) +
           code_gen_reflection_parameters(effects.reflection);
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