#include "RenderEffect_Fresnel.h"

std::string fresnelParameters(const RenderEffect_Fresnel& fresnel)
{
    return "const float fresnel_strength = " + std::to_string(*fresnel.strenght) + ";\nconst vec3 fresnel_color = " + glm::to_string(*fresnel.col) + ";\n\n";
};