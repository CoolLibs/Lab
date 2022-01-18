#include "RenderEffectsManager.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Parameter/ParameterU.h>
#include <filesystem>
#include <ranges>
#include "CodeGen.h"
#include "RenderEffectParsing.h"

RenderEffectsManager::RenderEffectsManager(std::string_view render_effects_folder_path)
    : render_effects_folder_path{render_effects_folder_path}
    , render_effects{load_effect(render_effects_folder_path)}
{
}

RenderEffectsGestion load_effect(std::string_view render_effects_folder_path)
{
    RenderEffectsGestion effects_gestion;
    for (const auto& entry : std::filesystem::directory_iterator{render_effects_folder_path}) {
        if (entry.is_directory()) {
            std::vector<RenderEffect>& effects = entry.path().stem() == "Objects"
                                                     ? effects_gestion.render_effects_objects
                                                     : effects_gestion.render_effects_always;
            for (const auto& file : std::filesystem::directory_iterator{entry.path()}) {
                if (file.is_regular_file()) {
                    try {
                        RenderEffect render_effect;
                        render_effect.name = file.path().stem().string();
                        parse_render_effect(render_effect, Cool::File::to_string(file.path().string()));
                        effects.push_back(render_effect);
                    }
                    catch (const std::exception& e) {
                        Cool::Log::ToUser::warn("is0::RenderEffectsManager::" + file.path().stem().string(), "Failed to parse node from file '{}':\n{}", file.path().string(), e.what());
                    }
                }
            }
        }
    }
    return effects_gestion;
};

std::vector<RenderEffect> merge_effect(std::vector<RenderEffect> old_render_effect, std::vector<RenderEffect> new_render_effect)
{
    for (auto& effect : new_render_effect) {
        const auto effect_here = std::ranges::find_if(old_render_effect, [&](const RenderEffect& effect_here) {
            return effect_here.name == effect.name;
        });
        if (effect_here == old_render_effect.end()) {
        }
        else {
            effect.is_active = effect_here->is_active;
            Cool::ParameterList new_parameters{};
            new_parameters->reserve(effect_here->parameters->size());
            for (auto& param : *effect_here->parameters) {
                const auto desc = std::visit([&](auto&& param) { return Cool::Parameter::AnyDesc(param.description()); }, param);
                new_parameters->push_back(Cool::ParameterU::make_param(effect_here->parameters, desc));
            }
            effect.parameters = std::move(new_parameters);
        }
    }
    return new_render_effect;
}

RenderEffectsGestion merge(RenderEffectsGestion old_render_effects, RenderEffectsGestion new_render_effects)
{
    new_render_effects.render_effects_always  = merge_effect(old_render_effects.render_effects_always, new_render_effects.render_effects_always);
    new_render_effects.render_effects_objects = merge_effect(old_render_effects.render_effects_objects, new_render_effects.render_effects_objects);
    return new_render_effects;
}

RenderEffectsGestion RenderEffectsManager::reload()
{
    RenderEffectsGestion new_render_effect = load_effect(render_effects_folder_path);
    return merge(render_effects, new_render_effect);
    // update_render_effects(render_effects.render_effects_objects);
    // update_render_effects(render_effects.render_effects_always);
}

std::string code_gen_effects_parameters(const RenderEffectsManager& effects)
{
    std::string code = "";
    for (const auto& effect : effects.render_effects.render_effects_objects) {
        if (effect.is_active) {
            code += CodeGen::parameters_definitions(effect.parameters);
            code += "\n";
            code += effect.extra_code;
        }
    }
    for (const auto& effect : effects.render_effects.render_effects_always) {
        if (effect.is_active) {
            code += CodeGen::parameters_definitions(effect.parameters);
            code += "\n";
            code += effect.extra_code;
        }
    }
    return code;
};

std::string code_gen_effects_object(const RenderEffectsManager& effects)
{
    std::string code = "";
    for (const auto& effect : effects.render_effects.render_effects_objects) {
        if (effect.is_active) {
            code += effect.code_template;
            code += "\n";
        }
    }
    return code;
};

std::string code_gen_effects_always(const RenderEffectsManager& effects)
{
    std::string code = "";
    for (const auto& effect : effects.render_effects.render_effects_always) {
        if (effect.is_active) {
            code += effect.code_template;
            code += "\n";
        }
    }
    return code;
};

bool effect_imgui(RenderEffect& effect)
{
    ImGui::Text(effect.name.c_str());
    bool has_changed = false;
    ImGui::PushID(&effect);
    effect.parameters.imgui([&has_changed]() { has_changed |= true; });
    has_changed |= ImGui::Checkbox("Enabled", &effect.is_active);
    ImGui::PopID();
    return has_changed;
}

bool effect_imgui_window(RenderEffectsManager& effects)
{
    bool has_changed = false;
    ImGui::Begin("Shading");
    for (auto& param : effects.render_effects.render_effects_objects) {
        has_changed |= effect_imgui(param);
        ImGui::Separator();
    }
    for (auto& param : effects.render_effects.render_effects_always) {
        has_changed |= effect_imgui(param);
        ImGui::Separator();
    }
    ImGui::End();
    return has_changed;
}