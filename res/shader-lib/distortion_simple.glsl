// https://www.shadertoy.com/view/MdfXRs

vec2 distortion_simple(
    vec2 in_uv, float Effect_intensity,
    vec2 Center
)
{
    vec2  disVec = Center - in_uv;
    float l      = length(disVec);
    float ll     = 1. - l * l;
    return mix(in_uv, Center - disVec * ll, Effect_intensity);
}