#include "RenderEffect_Fresnel.h"

std::string code_gen_fresnel_parameters(const RenderEffect_Fresnel& fresnel)
{
    return "const float fresnel_strength = " +
           std::to_string(*fresnel.strenght) +
           ";\nconst vec3 fresnel_color = " +
           glm::to_string(*fresnel.col) + ";\n\n";
};

std::string fresnelAdd()
{
    return R"(
        float fresnel = pow(clamp(1. - dot(normal, -rd), 0., 1.), fresnel_strength);
        finalCol += fresnel * fresnel_color;
    )";
}