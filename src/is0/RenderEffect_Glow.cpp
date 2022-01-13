#include "RenderEffect_Glow.h"

std::string glowParameters(const RenderEffect_Glow& glow)
{
    return "const float glow_strength = " + std::to_string(*glow.strenght) + ";\nconst vec3 glow_color = " + glm::to_string(*glow.col) + ";\n\n";
};

std::string glowAdd()
{
    return R"(
        float glow = pow(pas*glow_strength,2);
        finalCol += glow * glow_color;
    )";
};