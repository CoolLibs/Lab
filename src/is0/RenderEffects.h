#pragma once
#include "RenderEffect_Fresnel.h"
#include "RenderEffect_Glow.h"

struct RenderEffects {
    RenderEffect_Fresnel _fresnel;
    RenderEffect_Glow    _glow;
};

std::string effectsParameters(const RenderEffects& effects);