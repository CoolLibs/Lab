// #include "_ROOT_FOLDER_/shader-lib/luminance.glsl"

RgbColor bichrome(
    RgbColor in_color, float effect_intensity,
    RgbColor color1, RgbColor color2
)
{
    float    luminance = luminance(in_color);
    RgbColor out_color = mix(color1, color2, luminance);
    return mix(in_color, out_color, effect_intensity);
}