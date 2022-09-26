#pragma once

#include <Cool/Parameter/Parameter.h>
#include "RenderEffect.h"

struct RenderEffects {
    Cool::Parameter::Int bounces_count{
        {.name          = "Bounces Count",
         .default_value = 2,
         .min_value     = 0,
         .max_value     = 5}};
    std::vector<RenderEffect> for_objects;
    std::vector<RenderEffect> post_processing;
    std::vector<BaseCode>     normal;
    std::vector<BaseCode>     ray_marching;
    std::vector<BaseCode>     background;
    size_t                    normal_index     = 0;
    size_t                    ray_index        = 0;
    size_t                    background_index = 0;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Objects", for_objects),
            cereal::make_nvp("PostProcessing", post_processing),
            cereal::make_nvp("Normal", normal),
            cereal::make_nvp("RayMarcher", ray_marching),
            cereal::make_nvp("Background", background),
            cereal::make_nvp("NormalIndex", normal_index),
            cereal::make_nvp("RayIndex", ray_index),
            cereal::make_nvp("BackgroundIndex", background_index)
        );
    }
};

RenderEffects load_effects(std::filesystem::path render_effects_folder_path);
RenderEffects merge(const RenderEffects& old_render_effects, RenderEffects new_render_effects);
RenderEffects reload_effects(std::filesystem::path render_effects_folder_path, const RenderEffects& render_effects);

class RenderEffectsManager {
public:
    RenderEffects         render_effects;
    std::filesystem::path render_effects_folder_path;
    explicit RenderEffectsManager(std::filesystem::path render_effects_folder_path);

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

std::string code_gen_base_code(const BaseCode& base_code);

std::string code_gen_render_effects_extra_code(const RenderEffects& effects);
std::string code_gen_render_effects(const std::vector<RenderEffect>& render_effects);

bool base_code_imgui(BaseCode& base_code);
bool effect_imgui(RenderEffect& render_effect);
bool get_index_imgui(const std::vector<BaseCode>& base_code, size_t& index);
bool parameters_imgui(std::vector<BaseCode>& base_code, size_t& index);
bool effect_imgui_window(RenderEffects& effects);