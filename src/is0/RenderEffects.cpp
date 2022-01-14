#include "RenderEffects.h"

std::string add_effects_parameters(const RenderEffects& effects)
{
    return code_gen_fresnel_parameters(effects.fresnel) +
           code_gen_glow_parameters(effects.glow) +
           code_gen_reflection_parameters(effects.reflection);
};

std::string code_gen_effects(const RenderEffects& effects)
{
    std::string code = "";
    if (effects.fresnel.is_active)
        code += code_gen_fresnel();
    if (effects.reflection.is_active)
        code += code_gen_reflection();
    code += "}";
    if (effects.glow.is_active)
        code += code_gen_glow();
    return code;
};