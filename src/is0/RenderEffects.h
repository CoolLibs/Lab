#pragma once
#include <Cool/ImGui/ImGuiWindow.h>
#include "RenderEffect_Fresnel.h"
#include "RenderEffect_Glow.h"
#include "RenderEffect_Reflection.h"
#include "RenderEffect_Smoke.h"
#include "RendererPBR.h"

struct RenderEffects {
    RenderEffect_Fresnel    fresnel;
    RenderEffect_Glow       glow;
    RenderEffect_Reflection reflection;
    RenderEffect_Smoke      smoke;
};

std::string code_gen_effects_parameters(const RenderEffects& effects);
std::string code_gen_effects_object(const RenderEffects& effects);
std::string code_gen_effects_world(const RenderEffects& effects);

bool effect_imgui_window(RenderEffects& effects, int& render, CodeGen::MaterialProperties& m, CodeGen::LightProperties& l);