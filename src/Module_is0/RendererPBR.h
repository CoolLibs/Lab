#pragma once

#include <Cool/Parameter/Parameter.h>

namespace CodeGen {

struct LightProperties {
    Cool::Parameter::Float lightIntensity{{
        .name          = "Intensity",
        .default_value = 1.0f,
        .min_value     = 0.0f,
        .max_value     = 10.0f,
    }};
    Cool::Parameter::Float RebondIntensity{{
        .name          = "Reflects",
        .default_value = 0.0f,
        .min_value     = 0.0f,
        .max_value     = 1.0f,
    }};
    Cool::Parameter::Vec3  lightDirection{{
         .name          = "Direction",
         .default_value = glm::vec3(2.0f),
         .min_value     = glm::vec3(-10.0f),
         .max_value     = glm::vec3(10.0f),
    }};
    Cool::Parameter::Color lightColor;
};

struct MaterialProperties {
    Cool::Parameter::Color MaterialBaseColor;
    Cool::Parameter::Color MaterialDiffuseColor;
    Cool::Parameter::Color MaterialSpecularColor;
    Cool::Parameter::Float roughtness{{
        .name          = "Roughness",
        .default_value = 0.5f,
        .min_value     = 0.01f,
        .max_value     = 1.0f,
    }};
};

std::string light_prop_codegen(const LightProperties& l);
std::string material_prop_codegen(const MaterialProperties& m);
std::string pbr_renderer_codegen(const LightProperties& light, const MaterialProperties& material);

bool material_imgui_window(MaterialProperties& m);
bool light_imgui_window(LightProperties& l);

} // namespace CodeGen