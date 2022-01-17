#include "RenderEffect_InigoAO.h"

std::string code_gen_InigoAO_parameter(const RenderEffect_InigoAO& InigoAO)
{
    return "const float InigoAO_size = " + std::to_string(*InigoAO.size) + ";\n\n";
};

std::string code_gen_InigoAO_fct()
{
    return R"(
    vec2 hash2(float n) { return fract(sin(vec2(n, n + 1.0)) * vec2(43758.5453123, 22578.1459123)); }

    float calcOcclusion(in vec3 pos, in vec3 nor, float ra)
    {
        float occ = 0.0;
        for (int i = 0; i < 64; i++) {
            float h   = 0.01 + 2.0 * pow(float(i) / 63.0, 2.0);
            vec2  an  = hash2(ra + float(i) * 13.1) * vec2(3.14159, 6.2831);
            vec3  dir = vec3(sin(an.x) * sin(an.y), sin(an.x) * cos(an.y), cos(an.x));
            dir *= sign(dot(dir, nor));
            occ += clamp(5.0 * is0_main_sdf(pos + h * dir).x / h, -1.0, 1.0);
        }
        return clamp(occ / 64.0, 0.0, 1.0);
    }
)";
};

std::string code_gen_InigoAO()
{
    return R"(
    finalCol = finalCol * calcOcclusion(p, getNormal(p), 1.0);
    )";
};

bool InigoAO_imgui(RenderEffect_InigoAO& InigoAO)
{
    ImGui::Text("Inigo AO");
    bool has_changed = InigoAO.size.imgui();
    has_changed |= ImGui::Checkbox("Inigo AO Active", &InigoAO.is_active);
    return has_changed;
};