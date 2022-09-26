#include "RenderEffectsManager.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Parameter/ParameterU.h>
#include <filesystem>
#include <ranges>
#include "BaseCodeParsing.h"
#include "CodeGen.h"

RenderEffectsManager::RenderEffectsManager(std::filesystem::path render_effects_folder_path)
    : render_effects{load_effects(render_effects_folder_path)}
    , render_effects_folder_path{render_effects_folder_path}
{
}

template<typename T>
static void load_code(std::vector<T>& code, const std::filesystem::directory_entry& entry)
{
    static_assert(std::is_same_v<T, BaseCode> || std::is_same_v<T, RenderEffect>, "Only BaseCode and RenderEffect are supported");
    for (const auto& file : std::filesystem::directory_iterator{entry.path()})
    {
        if (file.is_regular_file())
        {
            try
            {
                T params;

                const auto parse_into = [&](auto&& destination) {
                    destination.name        = file.path().stem().string();
                    const auto file_content = Cool::File::to_string(file.path().string());
                    if (!file_content)
                    {
                        Cool::Log::ToUser::warning(
                            "RenderEffectsManager::load_code()",
                            file_content.error()
                        );
                        return;
                    }
                    parse_base_code(destination, *file_content);
                };
                if constexpr (std::is_same_v<T, BaseCode>)
                    parse_into(params);
                else // T is RenderEffect
                    parse_into(params.base);
                code.push_back(params);
            }
            catch (const std::exception& e)
            {
                Cool::Log::ToUser::warning(
                    "is0::RenderEffectsManager::" + file.path().stem().string(),
                    fmt::format("Failed to parse effect, normal or ray marching from file '{}':\n{}", file.path().string(), e.what())
                );
            }
        }
    }
}

RenderEffects load_effects(std::filesystem::path render_effects_folder_path)
{
    RenderEffects effects_gestion;
    for (const auto& entry : std::filesystem::directory_iterator{render_effects_folder_path})
    {
        if (entry.is_directory())
        {
            if (entry.path().stem() == "Objects" || entry.path().stem() == "PostProcessing")
            {
                std::vector<RenderEffect>& effects = entry.path().stem() == "Objects"
                                                         ? effects_gestion.for_objects
                                                         : effects_gestion.post_processing;
                load_code(effects, entry);
            }
            else if (entry.path().stem() == "Normals" || entry.path().stem() == "RayMarching" || entry.path().stem() == "Backgrounds")
            {
                std::vector<BaseCode>& param = entry.path().stem() == "Normals"
                                                   ? effects_gestion.normal
                                               : entry.path().stem() == "RayMarching"
                                                   ? effects_gestion.ray_marching
                                                   : effects_gestion.background;
                load_code(param, entry);
            }
        }
    }
    return effects_gestion;
}

template<typename T>
static std::vector<T> merge_code(const std::vector<T>& old_code, std::vector<T> new_code)
{
    static_assert(std::is_same_v<T, BaseCode> || std::is_same_v<T, RenderEffect>, "Only BaseCode and RenderEffect are supported");
    for (auto& code : new_code)
    {
        const auto code_here = std::find_if(old_code.begin(), old_code.end(), [&code](const T& code_here) {
            if constexpr (std::is_same_v<T, BaseCode>)
            {
                return code_here.name == code.name;
            }
            else // T is RenderEffect
            {
                return code_here.base.name == code.base.name;
            }
        });

        // waiting for xcode to support std::ranges::find_if
        //  const auto code_here = std::ranges::find_if(old_code, [&](const T& code_here) {
        //      if constexpr (std::is_same_v<T, BaseCode>)
        //      {
        //          return code_here.name == code.name;
        //      }
        //      else // T is RenderEffect
        //      {
        //          return code_here.base.name == code.base.name;
        //      }
        //  });
        if (code_here != old_code.end())
        {
            if constexpr (std::is_same_v<T, BaseCode>)
            {
                code.parameters = Cool::ParameterU::update_parameters(*code.parameters, code_here->parameters);
            }
            else // T is RenderEffect
            {
                code.is_active       = code_here->is_active;
                code.base.parameters = Cool::ParameterU::update_parameters(*code.base.parameters, code_here->base.parameters);
            }
        }
    }
    return new_code;
}

static size_t merge_index(const BaseCode& old_parameter, const std::vector<BaseCode>& new_parameters)
{
    const auto parameter_here = std::find_if(new_parameters.begin(), new_parameters.end(), [&](const BaseCode& parameter_here) {
        return parameter_here.name == old_parameter.name;
    });

    // waiting for xcode to support std::ranges::find_if
    // const auto parameter_here = std::ranges::find_if(new_parameters, [&](const BaseCode& parameter_here) {
    //     return parameter_here.name == old_parameter.name;
    // });
    if (parameter_here != new_parameters.end())
    {
        return std::distance(new_parameters.begin(), parameter_here);
    }
    else
    {
        return 0;
    }
}

RenderEffects merge(const RenderEffects& old_render_effects, RenderEffects new_render_effects)
{
    new_render_effects.post_processing  = merge_code(old_render_effects.post_processing, new_render_effects.post_processing);
    new_render_effects.for_objects      = merge_code(old_render_effects.for_objects, new_render_effects.for_objects);
    new_render_effects.normal           = merge_code(old_render_effects.normal, new_render_effects.normal);
    new_render_effects.ray_marching     = merge_code(old_render_effects.ray_marching, new_render_effects.ray_marching);
    new_render_effects.background       = merge_code(old_render_effects.background, new_render_effects.background);
    new_render_effects.normal_index     = merge_index(old_render_effects.normal[old_render_effects.normal_index], new_render_effects.normal);
    new_render_effects.ray_index        = merge_index(old_render_effects.ray_marching[old_render_effects.ray_index], new_render_effects.ray_marching);
    new_render_effects.background_index = merge_index(old_render_effects.background[old_render_effects.background_index], new_render_effects.background);
    return new_render_effects;
}

RenderEffects reload_effects(std::filesystem::path render_effects_folder_path, const RenderEffects& render_effects)
{
    return merge(render_effects, load_effects(render_effects_folder_path));
}

std::string code_gen_render_effects_extra_code(const RenderEffects& effects)
{
    std::string code = "";
    for (const auto& effect : effects.for_objects)
    {
        if (effect.is_active)
        {
            code += effect.base.extra_code;
        }
    }
    for (const auto& effect : effects.post_processing)
    {
        if (effect.is_active)
        {
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
    for (const auto& effect : render_effects)
    {
        if (effect.is_active)
        {
            code += "{\n";
            code += code_gen_base_code(effect.base);
            code += "}\n";
        }
    }
    return code;
}

bool base_code_imgui(BaseCode& base_code)
{
    bool has_changed = false;
    ImGui::PushID(&base_code);
    base_code.parameters.imgui([&has_changed]() { has_changed = true; });
    ImGui::PopID();
    return has_changed;
}

bool effect_imgui(RenderEffect& render_effect)
{
    bool has_changed = false;
    ImGui::Text("%s", render_effect.base.name.c_str());
    ImGui::PushID(&render_effect);
    has_changed |= ImGui::Checkbox("Enabled", &render_effect.is_active);
    ImGui::PopID();
    if (render_effect.is_active)
    {
        has_changed |= base_code_imgui(render_effect.base);
    }
    return has_changed;
}

bool get_index_imgui(const std::vector<BaseCode>& base_code, size_t& index)
{
    bool        has_changed         = false;
    const char* combo_preview_value = base_code[index].name.c_str();
    if (ImGui::BeginCombo("Select", combo_preview_value))
    {
        for (size_t n = 0; n < base_code.size(); n++)
        {
            const bool is_selected = (index == n);
            if (ImGui::Selectable(base_code[n].name.c_str(), is_selected))
            {
                index       = n;
                has_changed = true;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    return has_changed;
}

bool parameters_imgui(std::vector<BaseCode>& base_code, size_t& index)
{
    bool has_changed = false;
    ImGui::Text("%s", base_code[index].name.c_str());
    has_changed |= get_index_imgui(base_code, index);
    has_changed |= base_code_imgui(base_code[index]);
    return has_changed;
}

bool effect_imgui_window(RenderEffects& effects)
{
    bool has_changed = false;
    ImGui::Begin("Shading");
    effects.bounces_count.imgui({}, [&]() { has_changed = true; });
    ImGui::Separator();
    for (auto& param : effects.for_objects)
    {
        has_changed |= effect_imgui(param);
        ImGui::Separator();
    }
    for (auto& param : effects.post_processing)
    {
        has_changed |= effect_imgui(param);
        ImGui::Separator();
    }
    ImGui::End();
    ImGui::Begin("Normal");
    has_changed |= parameters_imgui(effects.normal, effects.normal_index);
    ImGui::End();
    ImGui::Begin("RayMarcher");
    has_changed |= parameters_imgui(effects.ray_marching, effects.ray_index);
    ImGui::End();
    ImGui::Begin("Background");
    has_changed |= parameters_imgui(effects.background, effects.background_index);
    ImGui::End();
    return has_changed;
}