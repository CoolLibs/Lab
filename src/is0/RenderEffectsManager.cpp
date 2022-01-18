#include "RenderEffectsManager.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <filesystem>
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
    effects_gestion.render_effects_objects.reserve(100);
    effects_gestion.render_effects_world.reserve(100);
    for (const auto& entry : std::filesystem::directory_iterator{render_effects_folder_path}) {
        if (entry.is_directory()) {
            std::vector<RenderEffect>& effects = entry.path().stem() == "Objects"
                                                     ? effects_gestion.render_effects_objects
                                                     : effects_gestion.render_effects_world;
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
    for (const auto& effect : effects.render_effects.render_effects_world) {
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

std::string code_gen_effects_world(const RenderEffectsManager& effects)
{
    std::string code = "";
    for (const auto& effect : effects.render_effects.render_effects_world) {
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
    for (auto& param : effects.render_effects.render_effects_world) {
        has_changed |= effect_imgui(param);
        ImGui::Separator();
    }
    ImGui::End();
    return has_changed;
}