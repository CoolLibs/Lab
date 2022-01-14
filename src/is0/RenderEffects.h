#pragma once
#include "RenderEffect_Fresnel.h"
#include "RenderEffect_Glow.h"
#include "RenderEffect_Reflection.h"

struct RenderEffects {
    RenderEffect_Fresnel    fresnel;
    RenderEffect_Glow       glow;
    RenderEffect_Reflection reflection;
};

std::string add_effects_parameters(const RenderEffects& effects);
std::string addParameters(const RenderEffects& effects);