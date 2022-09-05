// https://www.shadertoy.com/view/tdXfW7

vec2 wormhole(
    vec2 in_uv, float effect_intensity,
    vec2 center, float atractivity, float symmetry
)
{
    vec2 uv = in_uv - center;

    vec2 tuv = vec2(atan(uv.x, uv.y) / PI * symmetry, atractivity / length(uv)) + .5;
    tuv      = abs(1. - 2. * tuv);

    return mix(in_uv, tuv, effect_intensity);
}