#pragma once
#include <Cool/ImGui/ImGuiWindow.h>
#include "RenderEffect.h"
// #include "RenderEffect_Fresnel.h"
// #include "RenderEffect_Glow.h"
// #include "RenderEffect_Reflection.h"

struct RenderEffects {
    RenderEffect effect;
};

std::string code_gen_effects_parameters(const RenderEffects& effects);
std::string code_gen_effects_object(const RenderEffects& effects);
std::string code_gen_effects_world(const RenderEffects& effects);

bool effect_imgui_window(RenderEffects& effects);