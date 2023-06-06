// https://www.shadertoy.com/view/ltB3zD
// Gold Noise ©2015 dcerisano@standard3d.com
// - based on the Golden Ratio
// - uniform normalized distribution
// - fastest static noise generator function (also runs at low precision)
// - use with indicated fractional seeding method.

float golden_noise(in vec2 xy, in float seed)
{
    const float golden_ratio = 1.61803398874989484820459;
    return fract(tan(distance(xy * golden_ratio * 2000., xy) * seed) * xy.x);
}

vec2 golden_noise_uv(
    vec2 in_uv, float mask,
    float seed
)
{
    return mix(in_uv, vec2(golden_noise(in_uv, seed)), mask);
}

vec3 golden_noise_color(
    vec2 in_uv, float mask,
    float seed
)
{
    return vec3(
        golden_noise(in_uv, seed + 0.1),
        golden_noise(in_uv, seed + 0.2),
        golden_noise(in_uv, seed + 0.3)
    );
}