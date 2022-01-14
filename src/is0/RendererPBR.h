#pragma once

#include <Cool/Parameter/Parameter.h>

namespace CodeGen {

struct LightProperties {
    Cool::Parameter::Float lightIntensity{{.name          = "Intensity",
                                           .default_value = 1.0f,
                                           .min_value     = 0.0f,
                                           .max_value     = 10.0f}};
    Cool::Parameter::Float RebondIntensity{{.name          = "Reflects",
                                            .default_value = 0.0f,
                                            .min_value     = 0.0f,
                                            .max_value     = 1.0f}};
    Cool::Parameter::Vec3  lightDirection{{.name          = "Direction",
                                          .default_value = glm::vec3(2.0f),
                                          .min_value     = glm::vec3(-10.0f),
                                          .max_value     = glm::vec3(10.0f)}};
    Cool::Parameter::Color lightColor;
};

struct MaterialProperties {
    Cool::Parameter::Color MaterialBaseColor;
    Cool::Parameter::Color MaterialDiffuseColor;
    Cool::Parameter::Color MaterialSpecularColor;
    Cool::Parameter::Float roughtness{{.name          = "Roughness",
                                       .default_value = 0.5f,
                                       .min_value     = 0.01f,
                                       .max_value     = 1.0f}};
};

std::string lightPropCodeGen(const LightProperties& l);
std::string MaterialPropCodeGen(const MaterialProperties& m);
std::string PBRRendererCodeGen(const LightProperties& light, const MaterialProperties& material);

} // namespace CodeGen