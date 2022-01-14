#include "RenderEffect_Glow.h"

std::string glow_parameters(const RenderEffect_Glow& glow)
{
    return "const float glow_strength = " +
           std::to_string(*glow.strenght) +
           ";\nconst vec3 glow_color = " +
           glm::to_string(*glow.col) + ";\n\n";
};

std::string glowAdd()
{
    return R"(
        float glow = pow(iteration * glow_strength, 2.);
        finalCol += glow * glow_color;
    )";
};