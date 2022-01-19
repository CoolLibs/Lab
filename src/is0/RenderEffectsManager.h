#pragma once

#include "RenderEffect.h"

struct RenderEffectsGestion {
    std::vector<RenderEffect> _render_effects_objects;
    std::vector<RenderEffect> _render_effects_world;
};

class RenderEffectsManager {
public:
    RenderEffectsGestion _render_effects;
    std::string          _render_effects_folder_path;
    explicit RenderEffectsManager(std::string_view render_effects_folder_path);
};

RenderEffectsGestion load_effect(std::string_view render_effects_folder_path);
std::string          code_gen_effects_parameters(const RenderEffectsManager& effects);
std::string          code_gen_effects_object(const RenderEffectsManager& effects);
std::string          code_gen_effects_world(const RenderEffectsManager& effects);

bool effect_imgui(RenderEffect& effect);
bool effect_imgui_window(RenderEffectsManager& effects);