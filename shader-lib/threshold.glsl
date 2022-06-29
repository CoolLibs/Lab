// #include "_ROOT_FOLDER_/shader-lib/luminance.glsl"

RgbColor threshold(
    RgbColor in_color, float effect_intensity,
    int number_of_colors, RgbColor color1, RgbColor color2
)
{
    float luminance = luminance(in_color);
    luminance       = clamp(luminance, 0., 0.999); // Make sure that pure white doesn't get mapped to a range of its own by the floor()
    luminance *= (number_of_colors);
    luminance = floor(luminance);
    luminance /= (number_of_colors - 1);

    RgbColor out_color = mix(color1, color2, luminance);

    return mix(in_color, out_color, effect_intensity);
}