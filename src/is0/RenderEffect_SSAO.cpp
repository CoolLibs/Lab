#include "RenderEffect_SSAO.h"

std::string code_gen_ssao_parameter(const RenderEffect_SSAO& SSAO)
{
    return "const float SSAO_size = " + std::to_string(*SSAO.size) + ";\n\n";
};

std::string code_gen_ssao_function()
{
    return R"(
    vec3 BigRand[32] = {
        vec3(0.656178, 0.785943, 0.0918607),
        vec3(0.0980255, 0.770562, 0.888882),
        vec3(0.353252, 0.255623, 0.0786767),
        vec3(0.618091, 0.510575, 0.594409),
        vec3(0.0362255, 0.71392, 0.369793),
        vec3(0.948057, 0.122684, 0.21366),
        vec3(0.986175, 0.0407117, 0.76928),
        vec3(0.248604, 0.0666524, 0.984375),
        vec3(0.972198, 0.96704, 0.662496),
        vec3(0.635456, 0.410657, 0.909421),
        vec3(0.894436, 0.695456, 0.351756),
        vec3(0.990631, 0.0898465, 0.475967),
        vec3(0.0231635, 0.0941496, 0.698111),
        vec3(0.812677, 0.327525, 0.700522),
        vec3(0.582232, 0.955535, 0.728751),
        vec3(0.650258, 0.0143742, 0.585223),
        vec3(0.0383618, 0.523209, 0.759117),
        vec3(0.446425, 0.650929, 0.331828),
        vec3(0.106143, 0.437605, 0.248207),
        vec3(0.710746, 0.0986663, 0.133061),
        vec3(0.974395, 0.309458, 0.915311),
        vec3(0.729423, 0.975402, 0.962401),
        vec3(0.126102, 0.0279855, 0.415845),
        vec3(0.291726, 0.862941, 0.545305),
        vec3(0.909604, 0.409406, 0.475478),
        vec3(0.0169683, 0.193884, 0.221442),
        vec3(0.388836, 0.962859, 0.175787),
        vec3(0.160375, 0.788018, 0.0573443),
        vec3(0.908658, 0.969634, 0.0231941),
        vec3(0.749901, 0.74514, 0.80697),
        vec3(0.943968, 0.634449, 0.644398),
        vec3(0.78722f, 0.353526, 0.0561541)};



    vec3 rotatePoints(vec3 p, vec3 pts)
    {
        vec3  normalSphere = vec3(0.0, 0.0, 1.0);
        vec3 normal = getNormal(p);
        vec3  axeRot       = cross(normal, normalSphere);
        float angle        = acos(dot(normalize(normal), normalize(normalSphere)));
        pts                = erot(pts, normalize(axeRot), -angle) + 0.05*SSAO_size * normal;
        return pts;
    }

    float ssao(vec3 p)
    {
        float bl = 0;
        for (int i = 0; i < 32; i++) {
            if (is0_main_sdf(p +  (rotatePoints(p, SSAO_size * (BigRand[i] - vec3(0.5, 0.5, 0.0))))) > 0) {
                bl += 1;
            }
        }
        return bl / 32;
    }
    )";
};

std::string code_gen_ssao()
{
    return R"(
    finalCol *= ssao(p);
    )";
};

bool ssao_imgui(RenderEffect_SSAO& SSAO)
{
    ImGui::Text("SSAO");
    bool has_changed = SSAO.size.imgui();
    has_changed |= ImGui::Checkbox("SSAO Active", &SSAO.is_active);
    return has_changed;
};