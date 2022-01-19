#pragma once

#include "RenderEffect.h"

struct RenderEffectsGestion {
    std::vector<RenderEffect> render_effects_objects;
    std::vector<RenderEffect> render_effects_always;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Objects", render_effects_objects),
                cereal::make_nvp("Always", render_effects_always));
    }
};

RenderEffectsGestion      load_effect(std::string_view render_effects_folder_path);
std::vector<RenderEffect> merge_effect(std::vector<RenderEffect> old_render_effect, std::vector<RenderEffect> new_render_effect);
RenderEffectsGestion      merge(RenderEffectsGestion old_render_effects, RenderEffectsGestion new_render_effects);

class RenderEffectsManager {
public:
    RenderEffectsGestion render_effects;
    std::string          render_effects_folder_path;
    explicit RenderEffectsManager(std::string_view render_effects_folder_path);
    RenderEffectsGestion reload();

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Render Effects", render_effects));
        render_effects = reload();
    }
};

std::string code_gen_effects_parameters(const RenderEffectsManager& effects);
std::string code_gen_effects(const std::vector<RenderEffect>& render_effects);

bool effect_imgui(RenderEffect& effect);
bool effect_imgui_window(RenderEffectsManager& effects);