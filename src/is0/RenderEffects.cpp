#include "RenderEffects.h"
#include "CodeGen.h"

std::string code_gen_effects_parameters(const RenderEffects& effects)
{
    // return code_gen_fresnel_parameters(effects.fresnel) +
    //        code_gen_glow_parameters(effects.glow) +
    //        code_gen_reflection_parameters(effects.reflection);
    return CodeGen::parameters_definitions(effects.effect.parameters);
};

std::string code_gen_effects_object(const RenderEffects& effects)
{
    // std::string code = "";
    // if (effects.fresnel.is_active) {
    //     code += code_gen_fresnel();
    // }
    // if (effects.reflection.is_active) {
    //     code += code_gen_reflection();
    // }
    // return code;
    return effects.effect.code_template;
};

std::string code_gen_effects_world(const RenderEffects& effects)
{
    // std::string code = "";
    // if (effects.glow.is_active) {
    //     code += code_gen_glow();
    // }
    // return code;
    return "";
};

bool effect_imgui_window(RenderEffects& effects)
{
    // ImGui::Begin("Shading");
    // bool has_changed = fresnel_imgui(effects.fresnel);
    // ImGui::Separator();
    // has_changed |= glow_imgui(effects.glow);
    // ImGui::Separator();
    // has_changed |= reflection_imgui(effects.reflection);
    // ImGui::End();
    // return has_changed;
    return false;
}