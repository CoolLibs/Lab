// https://www.shadertoy.com/view/MdfXRs

vec2 distortion_simple(
    vec2 in_uv, float effect_intensity,
    vec2 center, float strength
)
{
    vec2  disVec = center - in_uv;
    float l      = length(disVec);
    float ll     = 1. - l * l;
    return mix(in_uv, center - disVec * ll / (-strength + 1.), effect_intensity);
}