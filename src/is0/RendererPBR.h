#pragma once

#include <Cool/Parameter/ParameterList.h>
#include "NodeTemplate.h"
#include "NodeTree.h"

namespace CodeGen {

struct LightProperties {
    glm::vec3 lightDirection = glm::vec3(2.);
    glm::vec3 lightColor     = glm::vec3(1.);
    float     lightIntensity = 1.0;
};

struct MaterialProperties {
    glm::vec3 MaterialBaseColor     = glm::vec3(1.);
    glm::vec3 MaterialDiffuseColor  = glm::vec3(1.);
    glm::vec3 MaterialSpecularColor = glm::vec3(1.);
    float     roughtness            = 3.;
    float     roughtnessL           = (roughtness < .01) ? .01 : roughtness;
};

std::string lightPropCodeGen(const LightProperties& l);
std::string MaterialPropCodeGen(const MaterialProperties& m);
std::string PBRRendererCodeGen(const LightProperties& light, const MaterialProperties& material);

//TODO : updateLight()
//TODO : updateMaterial()

} // namespace CodeGen