#include "RenderEffect_Reflection.h"

std::string code_gen_reflection_parameters(const RenderEffect_Reflection& reflection)
{
    return "const float IOR = " +
           std::to_string(*reflection.strenght) +
           ";\n\n";
}

std::string code_gen_reflection()
{
    return "#include \"is0 shaders/reflection.glsl\"\n";
}

bool reflection_imgui(RenderEffect_Reflection& reflection)
{
    ImGui::Text("Reflection");
    bool has_changed = reflection.strenght.imgui();
    return has_changed;
}