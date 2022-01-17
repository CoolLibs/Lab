#include "RenderEffectsManager.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <filesystem>
#include "CodeGen.h"
#include "RenderEffectParsing.h"

RenderEffectsManager::RenderEffectsManager(std::string_view render_effects_folder_path)
    : _render_effects_folder_path{render_effects_folder_path}
{
    _render_effects = load_effect(_render_effects_folder_path);
}

std::vector<RenderEffect> load_effect(std::string_view render_effects_folder_path)
{
    std::vector<RenderEffect> render_effects;
    for (const auto& file : std::filesystem::directory_iterator{render_effects_folder_path}) {
        if (file.is_regular_file()) {
            try {
                RenderEffect render_effect;
                render_effect.name = file.path().stem().string();
                parse_render_effect(render_effect, Cool::File::to_string(file.path().string()));
                render_effects.push_back(render_effect);
            }
            catch (const std::exception& e) {
                Cool::Log::ToUser::warn("is0::RenderEffectsManager::" + file.path().stem().string(), "Failed to parse node from file '{}':\n{}", file.path().string(), e.what());
            }
        }
    }
    return render_effects;
};

std::string code_gen_effects_parameters(const RenderEffectsManager& effects)
{
    std::string code = "";
    for (const auto& effect : effects._render_effects) {
        if (effect.is_active) {
            code += CodeGen::parameters_definitions(effect.parameters);
            code += "\n";
        }
    }
    return code;
};

std::string code_gen_effects_object(const RenderEffectsManager& effects)
{
    std::string code = "";
    for (const auto& effect : effects._render_effects) {
        if (effect.is_active) {
            code += effect.code_template;
            code += "\n";
        }
    }
    return code;
};

std::string code_gen_effects_world(const RenderEffectsManager& effects)
{
    return "";
};

bool effect_imgui(RenderEffect& effect)
{
    ImGui::Text(effect.name.c_str());
    bool has_changed = false;
    effect.parameters.imgui([&has_changed]() { has_changed |= true; });
    has_changed |= ImGui::Checkbox("Enabled", &effect.is_active);
    Cool::Log::info(effect.is_active);
    return has_changed;
}

bool effect_imgui_window(RenderEffectsManager& effects)
{
    bool has_changed = false;
    ImGui::Begin("Shading");
    for (auto& param : effects._render_effects) {
        has_changed |= effect_imgui(param);
        ImGui::Separator();
    }
    ImGui::End();
    return has_changed;
}