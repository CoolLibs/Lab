#pragma once

#include "RenderEffect.h"

struct RenderEffects {
    std::vector<RenderEffect> for_objects;
    std::vector<RenderEffect> always_applied;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Objects", for_objects),
                cereal::make_nvp("Always", always_applied));
    }
};

RenderEffects             load_effects(std::string_view render_effects_folder_path);
std::vector<RenderEffect> merge_effects(const std::vector<RenderEffect>& old_render_effect, std::vector<RenderEffect> new_render_effect);
RenderEffects             merge(const RenderEffects& old_render_effects, RenderEffects new_render_effects);
RenderEffects             reload_effects(std::string_view render_effects_folder_path, const RenderEffects& render_effects);

class RenderEffectsManager {
public:
    RenderEffects render_effects;
    std::string   render_effects_folder_path;
    explicit RenderEffectsManager(std::string_view render_effects_folder_path);

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Render Effects", render_effects));
        render_effects = reload_effects(render_effects_folder_path, render_effects);
    }
};

std::string code_gen_render_effects_extra_code(const RenderEffects& effects);
std::string code_gen_render_effects(const std::vector<RenderEffect>& render_effects);

bool effect_imgui(RenderEffect& effect);
bool effect_imgui_window(RenderEffects& effects);