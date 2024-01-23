/*
 * Normalize UVs
 */

#ifndef COOL_COMPUTE_SHADER
layout(location = 0) in vec2 _uv;
#endif
uniform float _aspect_ratio;

vec2 normalize_uv_with_aspect_ratio(vec2 uv, float aspect_ratio)
{
    uv -= 0.5;
    uv.x *= aspect_ratio;
    return uv * 2.;
}

vec2 normalize_uv(vec2 uv)
{
    return normalize_uv_with_aspect_ratio(uv, _aspect_ratio);
}

#ifndef COOL_COMPUTE_SHADER
vec2 normalized_uv()
{
    return normalize_uv(_uv);
}
#endif

vec2 unnormalize_uv_with_aspect_ratio(vec2 uv, float aspect_ratio)
{
    uv /= 2.;
    uv.x /= aspect_ratio;
    uv += 0.5;
    return uv;
}

vec2 unnormalize_uv(vec2 uv)
{
    return unnormalize_uv_with_aspect_ratio(uv, _aspect_ratio);
}

/// Returns 1 when x is negative, 0 when x is positive, with a small transition around 0 to apply antialiasing.
float antialised_step(float x)
{
#ifndef COOL_COMPUTE_SHADER
    float m = 0.75 * fwidth(x);
    return smoothstep(m, -m, x);
#else
    return step(0., x);
#endif
}