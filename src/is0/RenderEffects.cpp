#include "RenderEffects.h"

std::string code_gen_effects_parameters(const RenderEffects& effects)
{
    return code_gen_fresnel_parameters(effects.fresnel) +
           code_gen_glow_parameters(effects.glow) +
           code_gen_reflection_parameters(effects.reflection) +
           code_gen_soft_shadow_parameter(effects.shadow) +
           code_gen_soft_shadow_function() +
           code_gen_ssao_parameter(effects.SSAO) +
           code_gen_ssao_function() +
           code_gen_inigo_ao_parameter(effects.InigoAO) +
           code_gen_inigo_ao_function() +
           code_gen_edges_parameter(effects.edges) +
           (effects.smoke.is_active ? CodeGen::code_gen_smoke_parameters(effects.smoke) + "#include \"is0 shaders/smoke.glsl\"\n\n" : "");
};

std::string code_gen_effects_object(const RenderEffects& effects)
{
    std::string code = "";
    if (effects.fresnel.is_active) {
        code += code_gen_fresnel();
    }
    if (effects.reflection.is_active) {
        code += code_gen_reflection();
    }
    if (effects.shadow.is_active) {
        code += code_gen_soft_shadow();
    }
    if (effects.SSAO.is_active) {
        code += code_gen_ssao();
    }
    if (effects.InigoAO.is_active) {
        code += code_gen_inigo_ao();
    }
    return code;
};

std::string code_gen_effects_world(const RenderEffects& effects)
{
    std::string code = "";
    if (effects.glow.is_active) {
        code += code_gen_glow();
    }
    if (effects.edges.is_active) {
        code += code_gen_edges();
    }
    return code;
};

bool effect_imgui_window(RenderEffects& effects)
{
    ImGui::Begin("Shading");
    bool has_changed = fresnel_imgui(effects.fresnel);
    ImGui::Separator();
    has_changed |= glow_imgui(effects.glow);
    ImGui::Separator();
    has_changed |= reflection_imgui(effects.reflection);
    ImGui::Separator();
    has_changed |= smoke_imgui_window(effects.smoke);
    ImGui::Separator();
    has_changed |= soft_shadow_imgui(effects.shadow);
    ImGui::Separator();
    has_changed |= ssao_imgui(effects.SSAO);
    ImGui::Separator();
    has_changed |= inigo_ao_imgui(effects.InigoAO);
    ImGui::Separator();
    has_changed |= edges_imgui(effects.edges);
    ImGui::End();
    return has_changed;
}