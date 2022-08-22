// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"

vec3 threshold(
    vec3 in_color, float effect_intensity,
    int number_of_colors, vec3 color1, vec3 color2
)
{
    float luminance = luminance(in_color);
    luminance       = clamp(luminance, 0., 0.999); // Make sure that pure white doesn't get mapped to a range of its own by the floor()
    luminance *= (number_of_colors);
    luminance = floor(luminance);
    luminance /= (number_of_colors - 1);

    vec3 out_color = mix(color1, color2, luminance);

    return mix(in_color, out_color, effect_intensity);
}