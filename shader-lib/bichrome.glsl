// #include "_ROOT_FOLDER_/shader-lib/luminance.glsl"

vec3 bichrome(
    vec3 in_color, float effect_intensity,
    vec3 color1, vec3 color2
)
{
    float luminance = luminance(in_color);
    vec3  out_color = mix(color1, color2, luminance);
    return mix(in_color, out_color, effect_intensity);
}