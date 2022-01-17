#include "RenderEffect_Edges.h"
std::string code_gen_edges_parameter(const RenderEffect_Edges& edges)
{
    return "const float edges_size = " + std::to_string(*edges.size) +
           ";\nconst vec3 edges_color = " + glm::to_string(*edges.color) + ";\n\n";
};

std::string code_gen_edges()
{
    return R"(
    if(res.edge == true){
        finalCol = edges_color ;
    }
    )";
};

bool edges_imgui(RenderEffect_Edges& edges)
{
    ImGui::Text("Edges");
    bool has_changed = edges.size.imgui();
    has_changed |= edges.color.imgui();
    has_changed |= ImGui::Checkbox("Edges Active", &edges.is_active);
    return has_changed;
};