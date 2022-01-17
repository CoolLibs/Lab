#pragma once
#include <Cool/ImGui/ImGuiWindow.h>
#include "RenderEffect_Edges.h"
#include "RenderEffect_Fresnel.h"
#include "RenderEffect_Glow.h"
#include "RenderEffect_InigoAO.h"
#include "RenderEffect_Reflection.h"
#include "RenderEffect_SSAO.h"
#include "RenderEffect_Smoke.h"
#include "RenderEffect_SoftShadow.h"

struct RenderEffects {
    RenderEffect_Fresnel    fresnel;
    RenderEffect_Glow       glow;
    RenderEffect_Reflection reflection;
    RenderEffect_Smoke      smoke;
    RenderEffect_Shadow     shadow;
    RenderEffect_SSAO       SSAO;
    RenderEffect_InigoAO    InigoAO;
    RenderEffect_Edges      edges;
};

std::string code_gen_effects_parameters(const RenderEffects& effects);
std::string code_gen_effects_object(const RenderEffects& effects);
std::string code_gen_effects_world(const RenderEffects& effects);

bool effect_imgui_window(RenderEffects& effects);