#include "RenderEffect_SoftShadow.h"

std::string SoftShadowParameters(const RenderEffect_Shadow& shadow)
{
    return "const float shadow_strength = " + std::to_string(*shadow.strenght) + ";\n\n";
};
std::string SoftShadowfct()
{
    return R"(
float softshadow2(in vec3 ro, in vec3 rd, float mint, float maxt, float k)
{   
    float res = 1.0;
    float ph  = 1e20;
    for (float t = mint; t < maxt;) {
        float h = is0_main_sdf(ro + rd * t);
        if (h < 0.001)
            return 0.0;
        float y = h * h / (2.0 * ph);
        float d = sqrt(h * h - y * y);
        res     = min(res, k * d / max(0.0, t - y));
        ph      = h;
        t += h;
    }
    return res;
}
)";
};

std::string SoftShadowAdd()
{
    return R"(
    vec3  sun  = vec3(100.0, 100.0, 100.0);
    vec3 lights[] = {sun};
    float shad = 1;
    for(int i = 0; i < lights.length(); i++ ) {
        shad = shad * softshadow2(p, normalize(lights[i] - p), 0.01, MAX_DIST, shadow_strength);
        }
    finalCol = finalCol * shad;
    )";
};
