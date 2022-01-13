#include "Renderer_Smoke.h"
#include <Cool/String/String.h>
#include <numeric>
#include <sstream>
#include "CodeGen.h"

namespace CodeGen {

std::string IntersVolumCodeGen(const SmokeProperties& p)
{
    std::stringstream IntersecVolum;

    IntersecVolum << R"(
    float IntersectVolumetric(in vec3 rayOrigin, in vec3 rayDirection, float maxT)
    {
        float precis = )";
    IntersecVolum << p.precis << ";\n";
    IntersecVolum << R"(
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

    return IntersecVolum.str();
};

std::string GetLightVisCodeGen()
{
    std::stringstream GetLightVis;

    GetLightVis << R"(
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

    return GetLightVis.str();
};

std::string Render(const SmokeProperties& p, const DefineVariables& d)
{
    std::stringstream Renderer;

    Renderer << R"(
vec3 render(in vec3 rayOrigin, in vec3 rayDirection)
{
    float depth       = LARGE_NUMBER;
    vec3  opaqueColor = vec3(0.3, 0.7, 0.98);
    
    float volumeDepth     = IntersectVolumetric(rayOrigin, rayDirection, depth);
    float opaqueVisiblity = )";
    Renderer << p.opaqueVisibility << ";\n";
    Renderer << R"(
    vec3  volumetricColor = vec3(0.0f);
    if (volumeDepth > 0.0) {
        const vec3  volumeAlbedo     = vec3(0.8);
        const float marchSize        = 0.6f * )";
    Renderer << d.MARCH_MULTIPLIER << ";\n";
    Renderer << R"(
        float       distanceInVolume = 0.0f;
        float       signedDistance   = 0.0;
        for (int i = 0; i < )";
    Renderer << d.MAX_VOLUME_MARCH_STEPS << "; i++) {\n";
    Renderer << R"(
            volumeDepth += max(marchSize, signedDistance);
            if (volumeDepth > depth || opaqueVisiblity < ABSORPTION_CUTOFF)
                break;

            vec3 position = rayOrigin + volumeDepth * rayDirection;

            signedDistance = is0_main_sdf(position);
            if (signedDistance < 0.0f) {
                distanceInVolume += marchSize;
                float previousOpaqueVisiblity = opaqueVisiblity;
                opaqueVisiblity *= BeerLambert()";
    Renderer << d.ABSORPTION_COEFFICIENT << " * GetFogDensity(position, signedDistance), marchSize);\n";
    Renderer << R"(
                float absorptionFromMarch = previousOpaqueVisiblity - opaqueVisiblity;)";

    /// LIGHT PART (WIP)
    Renderer << R"(
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
                })";
    Renderer << R"(
                volumetricColor += absorptionFromMarch * volumeAlbedo * GetAmbientLight();
            }
        }
    }
    return min(volumetricColor, 1.0f) + opaqueVisiblity * opaqueColor;
}
)";
    return Renderer.str();
}

std::string SmokeRenderer(const SmokeProperties& p, const DefineVariables& d)
{
    std::string res = IntersVolumCodeGen(p) + GetLightVisCodeGen() + Render(p, d);
    return res;
}

}; // namespace CodeGen