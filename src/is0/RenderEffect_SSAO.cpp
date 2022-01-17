#include "RenderEffect_SSAO.h"

std::string code_gen_SSAO_parameter(const RenderEffect_SSAO& SSAO)
{
    return "const float SSAO_size = " + std::to_string(*SSAO.size) + ";\n\n";
};

std::string code_gen_SSAO_fct()
{
    return R"(
    vec3 BigRand[32] = {
        vec3(0.656178f, 0.785943f, 0.0918607f),
        vec3(0.0980255f, 0.770562f, 0.888882f),
        vec3(0.353252f, 0.255623f, 0.0786767f),
        vec3(0.618091f, 0.510575f, 0.594409f),
        vec3(0.0362255f, 0.71392f, 0.369793f),
        vec3(0.948057f, 0.122684f, 0.21366f),
        vec3(0.986175f, 0.0407117f, 0.76928f),
        vec3(0.248604f, 0.0666524f, 0.984375f),
        vec3(0.972198f, 0.96704f, 0.662496f),
        vec3(0.635456f, 0.410657f, 0.909421f),
        vec3(0.894436f, 0.695456f, 0.351756f),
        vec3(0.990631f, 0.0898465f, 0.475967f),
        vec3(0.0231635f, 0.0941496f, 0.698111f),
        vec3(0.812677f, 0.327525f, 0.700522f),
        vec3(0.582232f, 0.955535f, 0.728751f),
        vec3(0.650258f, 0.0143742f, 0.585223f),
        vec3(0.0383618f, 0.523209f, 0.759117f),
        vec3(0.446425f, 0.650929f, 0.331828f),
        vec3(0.106143f, 0.437605f, 0.248207f),
        vec3(0.710746f, 0.0986663f, 0.133061f),
        vec3(0.974395f, 0.309458f, 0.915311f),
        vec3(0.729423f, 0.975402f, 0.962401f),
        vec3(0.126102f, 0.0279855f, 0.415845f),
        vec3(0.291726f, 0.862941f, 0.545305f),
        vec3(0.909604f, 0.409406f, 0.475478f),
        vec3(0.0169683f, 0.193884f, 0.221442f),
        vec3(0.388836f, 0.962859f, 0.175787f),
        vec3(0.160375f, 0.788018f, 0.0573443f),
        vec3(0.908658f, 0.969634f, 0.0231941f),
        vec3(0.749901f, 0.74514f, 0.80697f),
        vec3(0.943968f, 0.634449f, 0.644398f),
        vec3(0.787225f, 0.353526f, 0.0561541f)};

    vec3 rotateAxe(vec3 pts, vec3 axe, float angle)
    {
        axe     = normalize(axe);
        float c = cos(angle);
        float s = sin(angle);
        mat3  M = mat3(vec3(axe.x * axe.x * (1 - c) + c, axe.x * axe.y * (1 - c) + axe.z * s, axe.x * axe.z * (1 - c) - axe.y * s),
                    vec3(axe.x * axe.y * (1 - c) - axe.z * s, axe.y * axe.y * (1 - c) + c, axe.y * axe.z * (1 - c) + axe.x * s),
                    vec3(axe.x * axe.z * (1 - c) + axe.y * s, axe.y * axe.z * (1 - c) - axe.x * s, axe.z * axe.z * (1 - c) + c));
        return pts * M;
    }
    vec3 prodVect(vec3 v, vec3 u)
    {
        return vec3(v.y * u.z - v.z * u.y, v.z * u.x - v.x * u.z, v.x * u.y - v.y * u.x);
    }
    vec3 rotatePoints(vec3 p, vec3 pts)
    {
        vec3  normalSphere = vec3(0.0, 0.0, 1.0);
        vec3  axeRot       = prodVect(getNormal(p), normalSphere);
        float angle        = acos(dot(normalize(getNormal(p)), normalize(normalSphere)));
        pts                = rotateAxe(pts, axeRot, angle) + 0.05*SSAO_size * axeRot;
        return pts;
    }

    float ssao(vec3 p)
    {
        float bl = 0;
        for (int i = 0; i < 32; i++) {
            if (is0_main_sdf(p + 1 * (rotatePoints(p, SSAO_size * (BigRand[i] - vec3(0.5, 0.5, 0.0))))) > 0) {
                bl += 1;
            }
        }
        return bl / 32;
    }
    )";
};

std::string code_gen_SSAO()
{
    return R"(
    finalCol = finalCol * ssao(p);
    )";
};

bool SSAO_imgui(RenderEffect_SSAO& SSAO)
{
    ImGui::Text("SSAO");
    bool has_changed = SSAO.size.imgui();
    has_changed |= ImGui::Checkbox("SSAO Active", &SSAO.is_active);
    return has_changed;
};