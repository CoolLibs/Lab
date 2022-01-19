#include "RenderEffect_Fresnel.h"

std::string code_gen_fresnel_parameters(const RenderEffect_Fresnel& fresnel)
{
    return "const float fresnel_strength = " +
           std::to_string(*fresnel.strenght) +
           ";\nconst vec3 fresnel_color = " +
           glm::to_string(*fresnel.col) + ";\n\n";
}

std::string code_gen_fresnel()
{
    return R"(
        float fresnel = pow(clamp(1. - dot(normal, -rd), 0., 1.), fresnel_strength);
        finalCol += fresnel * fresnel_color;
    )";
}

bool fresnel_imgui(RenderEffect_Fresnel& fresnel)
{
    ImGui::Text("Fresnel");
    bool has_changed = fresnel.strenght.imgui();
    has_changed |= fresnel.col.imgui();
    //has_changed |= ImGui::Checkbox("Fresnel Active", &fresnel.is_active);
    return has_changed;
}