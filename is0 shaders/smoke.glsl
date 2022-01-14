// See https://wallisc.github.io/rendering/2020/05/02/Volumetric-Rendering-Part-2.html
// And https://www.shadertoy.com/view/tsScDG

/// NOISE PART ///

#define NUM_OCTAVES          3
#define LARGE_NUMBER         1e20
#define MAX_SDF_SPHERE_STEPS 15
#define ABSORPTION_CUTOFF    0.25

float noise(vec3 x)
{
    // grid
    vec3 p = floor(x);
    vec3 w = fract(x);

    // quintic interpolant
    vec3 u = w * w * w * (w * (w * 6.0 - 15.0) + 10.0);

    // gradients
    vec3 ga = hash_minus_1_to_1(p + vec3(0.0, 0.0, 0.0));
    vec3 gb = hash_minus_1_to_1(p + vec3(1.0, 0.0, 0.0));
    vec3 gc = hash_minus_1_to_1(p + vec3(0.0, 1.0, 0.0));
    vec3 gd = hash_minus_1_to_1(p + vec3(1.0, 1.0, 0.0));
    vec3 ge = hash_minus_1_to_1(p + vec3(0.0, 0.0, 1.0));
    vec3 gf = hash_minus_1_to_1(p + vec3(1.0, 0.0, 1.0));
    vec3 gg = hash_minus_1_to_1(p + vec3(0.0, 1.0, 1.0));
    vec3 gh = hash_minus_1_to_1(p + vec3(1.0, 1.0, 1.0));

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

/// RENDER PART ///

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

/// LIGHT PART (WIP) ///

#define LIGHT_ATTENUATION_FACTOR     1.65
#define NUM_LIGHT_COLORS             3
#define NUM_LIGHTS                   3
#define UNIFORM_LIGHT_SPEED          1
#define MAX_VOLUME_LIGHT_MARCH_STEPS 4

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
