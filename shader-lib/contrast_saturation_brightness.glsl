// #include "_ROOT_FOLDER_/shader-lib/luminance.glsl"

vec3 contrast_saturation_brightness(
    vec3 in_color, float effect_intensity,
    float contrast, float brightness, float saturation
)
{
    const vec3 average_luminosity = vec3(0.5);
    vec3       brightness_color   = in_color * brightness;
    vec3       intensity          = vec3(luminance(brightness_color));
    vec3       saturation_color   = mix(intensity, brightness_color, saturation);
    vec3       contrast_color     = mix(average_luminosity, saturation_color, contrast);

    vec3 out_color = contrast_color;
    return mix(in_color, out_color, effect_intensity);
}