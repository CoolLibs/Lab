#version 430

// SOURCE :
// https://wallisc.github.io/rendering/2020/05/02/Volumetric-Rendering-Part-2.html
// https://www.shadertoy.com/view/tsScDG

layout(location = 0) in vec2 _uv;
uniform float _time;
out vec4      out_Color;

#include "_COOL_RES_/shaders/camera.glsl"
#include "_COOL_RES_/shaders/math.glsl"

#define NUM_LIGHT_COLORS    3
#define NUM_LIGHTS          3
#define UNIFORM_LIGHT_SPEED 1

#define NUM_OCTAVES            3
#define LARGE_NUMBER           1e20
#define MAX_SDF_SPHERE_STEPS   15
#define ABSORPTION_COEFFICIENT 0.5

// Reduce value of the following variable to enhance performance
#define MAX_VOLUME_MARCH_STEPS       50
#define MAX_VOLUME_LIGHT_MARCH_STEPS 4
#define ABSORPTION_CUTOFF            0.25
#define MARCH_MULTIPLIER             1.8
#define LIGHT_ATTENUATION_FACTOR     1.65

/// LIGHT PART ///

struct OrbLight {
    vec3  Position;
    float Radius;
    vec3  LightColor;
};

vec3 GetLightColor(int lightIndex)
{
    return vec3(1.0, 1.0, 1.0);
}

OrbLight GetLight(int lightIndex)
{
    // Create an orb of the selected Index
    const float lightMultiplier = 17.0f;
#if UNIFORM_LIGHT_SPEED
    float theta  = _time * 0.7 + float(lightIndex) * 3.1415926 * 2.0 / float(NUM_LIGHT_COLORS);
    float radius = 18.5f;
#else
    float theta  = _time * 0.4 * (float(lightIndex) + 1.0f);
    float radius = 19.0f + float(lightIndex) * 2.0;
#endif

    OrbLight orb;
    orb.Position   = vec3(radius * cos(theta), 6.0 + sin(theta * 2.0) * 2.5, radius * sin(theta));
    orb.LightColor = GetLightColor(lightIndex) * lightMultiplier;
    orb.Radius     = 0.8f;

    return orb;
}

float GetLightAttenuation(float distanceToLight)
{
    // Get the atenuation of the light from a distance
    return 1.0 / pow(distanceToLight, LIGHT_ATTENUATION_FACTOR);
}

/// NOISE PART ///
vec3 hash(vec3 p)
{
    p = vec3(dot(p, vec3(127.1, 311.7, 74.7)),
             dot(p, vec3(269.5, 183.3, 246.1)),
             dot(p, vec3(113.5, 271.9, 124.6)));

    return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

float noise(vec3 x)
{
    // grid
    vec3 p = floor(x);
    vec3 w = fract(x);

    // quintic interpolant
    vec3 u = w * w * w * (w * (w * 6.0 - 15.0) + 10.0);

    // gradients
    vec3 ga = hash(p + vec3(0.0, 0.0, 0.0));
    vec3 gb = hash(p + vec3(1.0, 0.0, 0.0));
    vec3 gc = hash(p + vec3(0.0, 1.0, 0.0));
    vec3 gd = hash(p + vec3(1.0, 1.0, 0.0));
    vec3 ge = hash(p + vec3(0.0, 0.0, 1.0));
    vec3 gf = hash(p + vec3(1.0, 0.0, 1.0));
    vec3 gg = hash(p + vec3(0.0, 1.0, 1.0));
    vec3 gh = hash(p + vec3(1.0, 1.0, 1.0));

    // projections
    float va = dot(ga, w - vec3(0.0, 0.0, 0.0));
    float vb = dot(gb, w - vec3(1.0, 0.0, 0.0));
    float vc = dot(gc, w - vec3(0.0, 1.0, 0.0));
    float vd = dot(gd, w - vec3(1.0, 1.0, 0.0));
    float ve = dot(ge, w - vec3(0.0, 0.0, 1.0));
    float vf = dot(gf, w - vec3(1.0, 0.0, 1.0));
    float vg = dot(gg, w - vec3(0.0, 1.0, 1.0));
    float vh = dot(gh, w - vec3(1.0, 1.0, 1.0));

    // interpolation
    return va +
           u.x * (vb - va) +
           u.y * (vc - va) +
           u.z * (ve - va) +
           u.x * u.y * (va - vb - vc + vd) +
           u.y * u.z * (va - vc - ve + vg) +
           u.z * u.x * (va - vb - ve + vf) +
           u.x * u.y * u.z * (-va + vb + vc - vd + ve - vf - vg + vh);
}

float fbm(vec3 x, float H)
{
    float G = exp2(-H);
    float f = 1.0;
    float a = 1.0;
    float t = 0.0;
    for (int i = 0; i < NUM_OCTAVES; i++) {
        t += a * noise(f * x);
        f *= 2.0;
        a *= G;
    }
    return t;
}

/// SDF PART ///

vec3 Translate(vec3 pos, vec3 translate)
{
    return pos -= translate;
}

float sdSmoothUnion(float d1, float d2, float k)
{
    float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return mix(d2, d1, h) - k * h * (1.0 - h);
}

float sdPlane(vec3 p)
{
    return p.y;
}

float sdSphere(vec3 p, vec3 origin, float s)
{
    p = Translate(p, origin);
    return length(p) - s;
}

float QueryVolumetricDistanceField(in vec3 pos)
{
    // Replace sdf()
    vec3  fbmCoord = (pos + 2.0 * vec3(0.0, 0.0, 0.0)) / 1.5f; // change vecc3 to add movement
    float sdfValue = sdSphere(pos, vec3(-8.0, 2.0 + 20.0 * sin(_time), -1), 5.6);
    sdfValue       = sdSmoothUnion(sdfValue, sdSphere(pos, vec3(8.0, 8.0 + 12.0 * cos(_time), 3), 5.6), 3.0f);
    sdfValue       = sdSmoothUnion(sdfValue, sdSphere(pos, vec3(5.0 * sin(_time), 3.0, 0), 8.0), 3.0) + 7.0 * fbm(fbmCoord, 0.5);
    sdfValue       = sdSmoothUnion(sdfValue, sdPlane(pos + vec3(0, 0.4, 0)), 22.0);
    return sdfValue;
}

/// RENDER PART ///

float IntersectVolumetric(in vec3 rayOrigin, in vec3 rayDirection, float maxT)
{
    float precis = 0.5;
    float t      = 0.0f;
    for (int i = 0; i < MAX_SDF_SPHERE_STEPS; i++) {
        float result = QueryVolumetricDistanceField(rayOrigin + rayDirection * t);
        if (result < (precis) || t > maxT)
            break;
        t += result;
    }
    return (t >= maxT) ? -1.0 : t;
}

float BeerLambert(float absorption, float dist)
{
    return exp(-absorption * dist);
}

vec3 GetAmbientLight()
{
    return 0.7 * vec3(1, 1, 1);
}

float Luminance(vec3 color)
{
    return (color.r * 0.3) + (color.g * 0.59) + (color.b * 0.11);
}

bool IsColorInsignificant(vec3 color)
{
    const float minValue = 0.009;
    return Luminance(color) < minValue;
}

float GetFogDensity(vec3 position, float sdfDistance)
{
    const float maxSDFMultiplier = 1.0;
    bool        insideSDF        = sdfDistance < 0.0;
    float       sdfMultiplier    = insideSDF ? min(abs(sdfDistance), maxSDFMultiplier) : 0.0;

#if UNIFORM_FOG_DENSITY
    return sdfMultiplier;
#else
    return sdfMultiplier * abs(fbm(position / 6.0, 0.5));
#endif
}

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

        signedDistance = QueryVolumetricDistanceField(position);
        if (signedDistance < 0.0) {
            lightVisibility *= BeerLambert(ABSORPTION_COEFFICIENT * GetFogDensity(position, signedDistance), marchSize);
        }
    }
    return lightVisibility;
}

vec3 Render(in vec3 rayOrigin, in vec3 rayDirection)
{
    float depth       = LARGE_NUMBER;
    vec3  opaqueColor = vec3(0.0f);

    vec3  normal;
    float t;

    float volumeDepth     = IntersectVolumetric(rayOrigin, rayDirection, depth);
    float opaqueVisiblity = 1.0f;
    vec3  volumetricColor = vec3(0.0f);
    if (volumeDepth > 0.0) {
        const vec3  volumeAlbedo     = vec3(0.8);
        const float marchSize        = 0.6f * MARCH_MULTIPLIER;
        float       distanceInVolume = 0.0f;
        float       signedDistance   = 0.0;
        for (int i = 0; i < MAX_VOLUME_MARCH_STEPS; i++) {
            volumeDepth += max(marchSize, signedDistance);
            if (volumeDepth > depth || opaqueVisiblity < ABSORPTION_CUTOFF)
                break;

            vec3 position = rayOrigin + volumeDepth * rayDirection;

            signedDistance = QueryVolumetricDistanceField(position);
            if (signedDistance < 0.0f) {
                distanceInVolume += marchSize;
                float previousOpaqueVisiblity = opaqueVisiblity;
                opaqueVisiblity *= BeerLambert(ABSORPTION_COEFFICIENT * GetFogDensity(position, signedDistance), marchSize);
                float absorptionFromMarch = previousOpaqueVisiblity - opaqueVisiblity;

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

    return min(volumetricColor, 1.0f) + opaqueVisiblity * opaqueColor;
}

void main()
{
    vec3 ro = cool_ray_origin();
    vec3 rd = cool_ray_direction();

    out_Color = vec4(Render(ro, rd), 1.);
}