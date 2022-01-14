#include "RenderEffect_Smoke.h"
#include <Cool/String/String.h>
#include <numeric>
#include <sstream>
#include "CodeGen.h"

bool smoke_imgui_window(RenderEffect_Smoke& prop)
{
    ImGui::Text("Smoke");
    bool has_changed = ImGui::Checkbox("Smoke Active", &prop.is_active);
    has_changed |= prop.ABSORPTION_COEFFICIENT.imgui();
    has_changed |= prop.MARCH_MULTIPLIER.imgui();
    has_changed |= prop.MAX_VOLUME_MARCH_STEPS.imgui();
    return has_changed;
};

namespace CodeGen {

std::string SmokeParameters(const RenderEffect_Smoke& parameters)
{
    std::stringstream l;

    l << "#define ABSORPTION_COEFFICIENT " << std::to_string(*parameters.ABSORPTION_COEFFICIENT) << '\n';
    l << "#define MAX_VOLUME_MARCH_STEPS " << std::to_string(*parameters.MAX_VOLUME_MARCH_STEPS) << '\n';
    l << "#define MARCH_MULTIPLIER " << std::to_string(*parameters.MARCH_MULTIPLIER) << "\n\n";
    return l.str();
};

std::string IntersectVolumetricCodeGen()
{
    return R"(
float IntersectVolumetric(in vec3 rayOrigin, in vec3 rayDirection, float maxT)
{
    float precis = 0.5f;
    float t      = 0.0f;
    for (int i = 0; i < MAX_SDF_SPHERE_STEPS; i++) {
        float result = is0_main_sdf(rayOrigin + rayDirection * t);
        if (result < (precis) || t > maxT)
            break;
        t += result;
    }
    return (t >= maxT) ? -1.0 : t;
}
    )";
};

std::string GetLightVisiblityCodeGen()
{
    return R"(
float GetLightVisiblity(in vec3 rayOrigin, in vec3 rayDirection, in float maxT, in int maxSteps, in float marchSize) 
{
    float t               = 0.0f;
    float lightVisibility = 1.0f;
    float signedDistance  = 0.0;
    for (int i = 0; i < maxSteps; i++) {
        t += max(marchSize, signedDistance);
        if (t > maxT || lightVisibility < ABSORPTION_CUTOFF)
            break;
            
        vec3 position = rayOrigin + t * rayDirection;
        
        signedDistance = is0_main_sdf(position);
        if (signedDistance < 0.0) {
            lightVisibility *= BeerLambert(ABSORPTION_COEFFICIENT * GetFogDensity(position, signedDistance), marchSize);
        }
    };
    return lightVisibility;
}
    )";
};

std::string Render()
{
    return R"(
vec3 render(in vec3 rayOrigin, in vec3 rayDirection)
{
    float depth       = LARGE_NUMBER;
    vec3  opaqueColor = vec3(0.3, 0.7, 0.98);
    float opaqueVisibility = 1.0f;
    
    float volumeDepth     = IntersectVolumetric(rayOrigin, rayDirection, depth);
    vec3  volumetricColor = vec3(0.0f);
    if (volumeDepth > 0.0) {
        const vec3  volumeAlbedo     = vec3(0.8);
        const float marchSize        = 0.6f * MARCH_MULTIPLIER;
        float       distanceInVolume = 0.0f;
        float       signedDistance   = 0.0;
        for (int i = 0; i < MAX_VOLUME_MARCH_STEPS; i++) {
            volumeDepth += max(marchSize, signedDistance);
            if (volumeDepth > depth || opaqueVisibility < ABSORPTION_CUTOFF)
                break;

            vec3 position = rayOrigin + volumeDepth * rayDirection;

            signedDistance = is0_main_sdf(position);
            if (signedDistance < 0.0f) {
                distanceInVolume += marchSize;
                float previousopaqueVisibility = opaqueVisibility;
                opaqueVisibility *= BeerLambert(ABSORPTION_COEFFICIENT * GetFogDensity(position, signedDistance), marchSize);
                float absorptionFromMarch = previousopaqueVisibility - opaqueVisibility;


    /// LIGHT PART (WIP)

                for (int lightIndex = 0; lightIndex < NUM_LIGHTS; lightIndex++) {
                    float lightVolumeDepth = 0.0f;
                    vec3  lightDirection   = (GetLight(lightIndex).Position - position);
                    float lightDistance    = length(lightDirection);
                    lightDirection /= lightDistance;

                    vec3 lightColor = GetLight(lightIndex).LightColor * GetLightAttenuation(lightDistance);
                    if (IsColorInsignificant(lightColor))
                        continue;

                    const float lightMarchSize = 0.65f;
                    float       lightVisiblity = GetLightVisiblity(position, lightDirection, lightDistance, MAX_VOLUME_LIGHT_MARCH_STEPS, lightMarchSize);
                    volumetricColor += absorptionFromMarch * lightVisiblity * volumeAlbedo * lightColor;
                }
                volumetricColor += absorptionFromMarch * volumeAlbedo * GetAmbientLight();
            }
        }
    }
    return min(volumetricColor, 1.0f) + opaqueVisibility * opaqueColor;
}
)";
}

std::string SmokeRenderer(const RenderEffect_Smoke& p)
{
    return IntersectVolumetricCodeGen() +
           GetLightVisiblityCodeGen() +
           Render();
}

std::string addSmoke(const RenderEffect_Smoke& p)
{
    if (p.is_active) {
        return SmokeRenderer(p);
    }
    return "";
}

}; // namespace CodeGen