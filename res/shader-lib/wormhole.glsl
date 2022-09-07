// https://www.shadertoy.com/view/tdXfW7

// #include "_COOL_RES_/shaders/math.glsl"

vec2 wormhole(
    vec2 in_uv, float effect_intensity,
    vec2 center, float attractivity, float subdivision
)
{
    vec2 uv = in_uv - center;

    vec2 tuv = vec2(atan(uv.x, uv.y) / TAU * subdivision / 2., attractivity / length(uv)) + .5;
    tuv      = abs(1. - 2. * tuv);

    return mix(in_uv, tuv + center, effect_intensity);
}