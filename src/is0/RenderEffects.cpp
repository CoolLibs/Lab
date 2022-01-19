#include "RenderEffects.h"

std::string code_gen_effects_parameters(const RenderEffects& effects)
{
    return code_gen_fresnel_parameters(effects.fresnel) +
           code_gen_glow_parameters(effects.glow) +
           code_gen_reflection_parameters(effects.reflection) +
           (effects.smoke.is_active ? CodeGen::code_gen_smoke_parameters(effects.smoke) + "#include \"is0 shaders/smoke.glsl\"\n\n" : "");
}

std::string code_gen_effects_object(const RenderEffects& effects)
{
    std::string code = "";
    if (effects.fresnel.is_active) {
        code += code_gen_fresnel();
    }
    if (effects.reflection.is_active) {
        code += code_gen_reflection();
    }
    return code;
}

std::string code_gen_effects_world(const RenderEffects& effects)
{
    std::string code = "";
    if (effects.glow.is_active) {
        code += code_gen_glow();
    }
    return code;
}

bool effect_imgui_window(RenderEffects& effects, int& render, CodeGen::MaterialProperties& m, CodeGen::LightProperties& l)
{
    ImGui::Begin("Shading");

    bool has_changed = false;

    const char* items[] = {"Default", "Cool effects", "Smoke", "PBR"};
    has_changed |= ImGui::Combo("Renders", &render, items, IM_ARRAYSIZE(items));
    ImGui::Separator();

    switch (render) {
    case 0:
        break;
    case 1:
        has_changed |= fresnel_imgui(effects.fresnel);
        ImGui::Separator();
        has_changed |= reflection_imgui(effects.reflection);
        ImGui::Separator();
        has_changed |= glow_imgui(effects.glow);
        ImGui::Separator();
        break;
    case 2:
        has_changed |= smoke_imgui_window(effects.smoke);
        ImGui::Separator();
        break;
    case 3:
        has_changed |= CodeGen::material_imgui_window(m);
        ImGui::Separator();
        has_changed |= CodeGen::light_imgui_window(l);
        break;
    default:
        break;
    }

    ImGui::End();

    return has_changed;
}