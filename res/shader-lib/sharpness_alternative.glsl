
// from https://www.shadertoy.com/view/llXGWf

// Sharpness kernel
// -x   -x    -x
// -x  x*8+1  -x
// -x   -x    -x

vec3 sharpness_alternative(
    vec2 in_uv, float effect_intensity,
    float offset_strength, float sharpening_strength

)
{
    vec3 out_color = image(in_uv);
    vec3 blur      = out_color;
    blur += image(in_uv + vec2(0.0, 0.001 * offset_strength));
    blur += image(in_uv + vec2(0.001 * offset_strength, 0.0));
    blur += image(in_uv - vec2(0.0, 0.001 * offset_strength));
    blur += image(in_uv - vec2(0.001 * offset_strength, 0.0));

    blur += image(in_uv + vec2(0.001 * offset_strength)) / 2.0;
    blur += image(in_uv - vec2(0.001 * offset_strength)) / 2.0;
    blur += image(in_uv + vec2(0.001 * offset_strength, -0.001 * offset_strength)) / 2.0;
    blur += image(in_uv + vec2(-0.001 * offset_strength, 0.001 * offset_strength)) / 2.0;

    blur /= 7.0;
    float sharpness = (out_color - blur).r * sharpening_strength;
    out_color += sharpness;

    return mix(image(in_uv), out_color, effect_intensity);
}