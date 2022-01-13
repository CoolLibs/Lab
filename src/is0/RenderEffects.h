#pragma once
#include "RenderEffect_Fresnel.h"
#include "RenderEffect_Glow.h"
#include "RenderEffect_Reflection.h"

struct RenderEffects {
    RenderEffect_Fresnel    _fresnel;
    RenderEffect_Glow       _glow;
    RenderEffect_Reflection _reflection;
};

std::string effectsParameters(const RenderEffects& effects);
std::string addParameters(const RenderEffects& effects);