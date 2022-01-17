#include "RenderEffects.h"

std::string code_gen_effects_parameters(const RenderEffects& effects)
{
    return code_gen_fresnel_parameters(effects.fresnel) +
           code_gen_glow_parameters(effects.glow) +
           code_gen_reflection_parameters(effects.reflection) +
           code_gen_softShadow_parameter(effects.shadow) +
           code_gen_softShadow_fct() +
           code_gen_SSAO_parameter(effects.SSAO) +
           code_gen_SSAO_fct() +
           code_gen_InigoAO_parameter(effects.InigoAO) +
           code_gen_InigoAO_fct() +
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
        code += code_gen_softShadow();
    }
    if (effects.SSAO.is_active) {
        code += code_gen_SSAO();
    }
    if (effects.InigoAO.is_active) {
        code += code_gen_InigoAO();
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
    has_changed |= softShadow_imgui(effects.shadow);
    ImGui::Separator();
    has_changed |= SSAO_imgui(effects.SSAO);
    ImGui::Separator();
    has_changed |= InigoAO_imgui(effects.InigoAO);
    ImGui::Separator();
    has_changed |= Edges_imgui(effects.edges);
    ImGui::End();
    return has_changed;
}