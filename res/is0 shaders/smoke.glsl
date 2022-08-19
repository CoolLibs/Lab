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
    for (int i = 0; i < NUM_OCTAVES; i++)
    {
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

float GetFogDensity(vec3 position, float sdfDistance)
{
    const float maxSDFMultiplier = 1.0;
    bool        insideSDF        = sdfDistance < 0.0;
    float       sdfMultiplier    = insideSDF ? min(abs(sdfDistance), maxSDFMultiplier) : 0.0;

    // #ifdef UNIFORM_FOG_DENSITY
    //     return sdfMultiplier;
    // #else
    return sdfMultiplier * abs(fbm(position / 6.0, 0.5));
    // #endif
}
