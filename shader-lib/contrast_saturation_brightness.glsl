// #include "_ROOT_FOLDER_/shader-lib/luminance.glsl"

RgbColor contrast_saturation_brightness(
    RgbColor in_color, float effect_intensity,
    float contrast, float brightness, float saturation
)
{
    const vec3 average_luminosity = vec3(0.5);
    RgbColor   brightness_color   = in_color * brightness;
    RgbColor   intensity          = RgbColor(luminance(brightness_color));
    RgbColor   saturation_color   = mix(intensity, brightness_color, saturation);
    RgbColor   contrast_color     = mix(average_luminosity, saturation_color, contrast);

    RgbColor out_color = contrast_color;
    return mix(in_color, out_color, effect_intensity);
}