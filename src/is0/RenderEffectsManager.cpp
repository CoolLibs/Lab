#include "RenderEffectsManager.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Parameter/ParameterU.h>
#include <filesystem>
#include <ranges>
#include "BaseCodeParsing.h"
#include "CodeGen.h"

RenderEffectsManager::RenderEffectsManager(std::string_view render_effects_folder_path)
    : render_effects{load_effects(render_effects_folder_path)}
    , render_effects_folder_path{render_effects_folder_path}
{
}

static void load_render(std::vector<RenderEffect>& effects, const std::filesystem::directory_entry& entry)
{
    for (const auto& file : std::filesystem::directory_iterator{entry.path()}) {
        if (file.is_regular_file()) {
            try {
                RenderEffect render_effect;
                render_effect.base.name = file.path().stem().string();
                parse_base_code(render_effect.base, Cool::File::to_string(file.path().string()));
                effects.push_back(render_effect);
            }
            catch (const std::exception& e) {
                Cool::Log::ToUser::warn("is0::RenderEffectsManager::" + file.path().stem().string(), "Failed to parse effects from file '{}':\n{}", file.path().string(), e.what());
            }
        }
    }
}

static void load_base(std::vector<BaseCode>& param, const std::filesystem::directory_entry& entry)
{
    for (const auto& file : std::filesystem::directory_iterator{entry.path()}) {
        if (file.is_regular_file()) {
            try {
                BaseCode params;
                params.name = file.path().stem().string();
                parse_base_code(params, Cool::File::to_string(file.path().string()));
                param.push_back(params);
            }
            catch (const std::exception& e) {
                Cool::Log::ToUser::warn("is0::RenderEffectsManager::" + file.path().stem().string(), "Failed to parse normal or ray marching from file '{}':\n{}", file.path().string(), e.what());
            }
        }
    }
}

RenderEffects load_effects(std::string_view render_effects_folder_path)
{
    RenderEffects effects_gestion;
    for (const auto& entry : std::filesystem::directory_iterator{render_effects_folder_path}) {
        if (entry.is_directory()) {
            if (entry.path().stem() == "Objects" || entry.path().stem() == "Always") {
                std::vector<RenderEffect>& effects = entry.path().stem() == "Objects"
                                                         ? effects_gestion.for_objects
                                                         : effects_gestion.always_applied;
                load_render(effects, entry);
            }
            else if (entry.path().stem() == "Normals" || entry.path().stem() == "RayMarching" || entry.path().stem() == "Backgrounds") {
                std::vector<BaseCode>& param = entry.path().stem() == "Normals"
                                                   ? effects_gestion.normal
                                               : entry.path().stem() == "RayMarching"
                                                   ? effects_gestion.ray_marching
                                                   : effects_gestion.background;
                load_base(param, entry);
            }
        }
    }
    return effects_gestion;
}

std::vector<RenderEffect> merge_effects(const std::vector<RenderEffect>& old_render_effect, std::vector<RenderEffect> new_render_effect)
{
    for (auto& effect : new_render_effect) {
        const auto effect_here = std::ranges::find_if(old_render_effect, [&](const RenderEffect& effect_here) {
            return effect_here.base.name == effect.base.name;
        });
        if (effect_here != old_render_effect.end()) {
            effect.is_active       = effect_here->is_active;
            effect.base.parameters = Cool::ParameterU::update_parameters(*effect_here->base.parameters, effect_here->base.parameters);
        }
    }
    return new_render_effect;
}

std::vector<BaseCode> merge_base_code(const std::vector<BaseCode>& old_base_code, std::vector<BaseCode> new_base_code)
{
    for (auto& base : new_base_code) {
        const auto base_here = std::ranges::find_if(old_base_code, [&](const BaseCode& base_here) {
            return base_here.name == base.name;
        });
        if (base_here != old_base_code.end()) {
            base.parameters = Cool::ParameterU::update_parameters(*base_here->parameters, base_here->parameters);
        }
    }
    return new_base_code;
}

RenderEffects merge(const RenderEffects& old_render_effects, RenderEffects new_render_effects)
{
    new_render_effects.always_applied   = merge_effects(old_render_effects.always_applied, new_render_effects.always_applied);
    new_render_effects.for_objects      = merge_effects(old_render_effects.for_objects, new_render_effects.for_objects);
    new_render_effects.normal           = merge_base_code(old_render_effects.normal, new_render_effects.normal);
    new_render_effects.ray_marching     = merge_base_code(old_render_effects.ray_marching, new_render_effects.ray_marching);
    new_render_effects.background       = merge_base_code(old_render_effects.background, new_render_effects.background);
    new_render_effects.normal_index     = old_render_effects.normal_index;
    new_render_effects.ray_index        = old_render_effects.ray_index;
    new_render_effects.background_index = old_render_effects.background_index;
    return new_render_effects;
}

RenderEffects reload_effects(std::string_view render_effects_folder_path, const RenderEffects& render_effects)
{
    return merge(render_effects, load_effects(render_effects_folder_path));
}

std::string code_gen_render_effects_extra_code(const RenderEffects& effects)
{
    std::string code = "";
    for (const auto& effect : effects.for_objects) {
        if (effect.is_active) {
            code += effect.base.extra_code;
        }
    }
    for (const auto& effect : effects.always_applied) {
        if (effect.is_active) {
            code += effect.base.extra_code;
        }
    }
    return code;
}

std::string code_gen_base_code(const BaseCode& base_code)
{
    std::string code = "";
    code += CodeGen::parameters_definitions(base_code.parameters);
    code += base_code.code;
    return code;
}

std::string code_gen_render_effects(const std::vector<RenderEffect>& render_effects)
{
    std::string code = "";
    for (const auto& effect : render_effects) {
        if (effect.is_active) {
            code += code_gen_base_code(effect.base);
            code += '\n';
        }
    }
    return code;
}

bool base_code_imgui(BaseCode& base_code)
{
    ImGui::Text("%s", base_code.name.c_str());
    bool has_changed = false;
    ImGui::PushID(&base_code);
    base_code.parameters.imgui([&has_changed]() { has_changed = true; });
    ImGui::PopID();
    return has_changed;
}

bool get_index(std::vector<BaseCode>& base_code, int& index)
{
    bool        has_changed         = false;
    const char* combo_preview_value = base_code[index].name.c_str();
    if (ImGui::BeginCombo("Select", combo_preview_value)) {
        for (int n = 0; n < base_code.size(); n++) {
            const bool is_selected = (index == n);
            if (ImGui::Selectable(base_code[n].name.c_str(), is_selected)) {
                index       = n;
                has_changed = true;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    return has_changed;
}

bool effect_imgui_window(RenderEffects& effects)
{
    bool has_changed = false;
    ImGui::Begin("Shading");
    for (auto& param : effects.for_objects) {
        has_changed |= base_code_imgui(param.base);
        ImGui::PushID(&param);
        has_changed |= ImGui::Checkbox("Enabled", &param.is_active);
        ImGui::PopID();
        ImGui::Separator();
    }
    for (auto& param : effects.always_applied) {
        has_changed |= base_code_imgui(param.base);
        ImGui::PushID(&param);
        has_changed |= ImGui::Checkbox("Enabled", &param.is_active);
        ImGui::PopID();
        ImGui::Separator();
    }
    ImGui::End();
    ImGui::Begin("Normal");
    has_changed |= get_index(effects.normal, effects.normal_index);
    has_changed |= base_code_imgui(effects.normal[effects.normal_index]);
    ImGui::Separator();
    ImGui::End();
    ImGui::Begin("RayMarcher");
    has_changed |= get_index(effects.ray_marching, effects.ray_index);
    has_changed |= base_code_imgui(effects.ray_marching[effects.ray_index]);
    ImGui::Separator();
    ImGui::End();
    ImGui::Begin("Background");
    has_changed |= get_index(effects.background, effects.background_index);
    has_changed |= base_code_imgui(effects.background[effects.background_index]);
    ImGui::Separator();
    ImGui::End();
    return has_changed;
}