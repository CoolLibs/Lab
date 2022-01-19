#include "RenderEffect_Glow.h"

std::string code_gen_glow_parameters(const RenderEffect_Glow& glow)
{
    return "const float glow_strength = " +
           std::to_string(*glow.strenght) +
           ";\nconst vec3 glow_color = " +
           glm::to_string(*glow.col) + ";\n\n";
}

std::string code_gen_glow()
{
    return R"(
        float glow = pow(iterations_count * glow_strength, 2.);
        finalCol += glow * glow_color;
    )";
}

bool glow_imgui(RenderEffect_Glow& glow)
{
    ImGui::Text("Glow");
    bool has_changed = glow.strenght.imgui();
    has_changed |= glow.col.imgui();
    //has_changed |= ImGui::Checkbox("Glow Active", &glow.is_active);
    return has_changed;
}