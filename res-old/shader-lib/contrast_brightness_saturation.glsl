// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"

vec3 contrast_brightness_saturation(
    vec3 in_color, float mask,
    float contrast, float brightness, float saturation
)
{
    const vec3 average_luminosity = vec3(0.5);
    vec3       brightness_color   = in_color * (brightness + 1.);
    vec3       intensity          = vec3(cool__luminance(brightness_color));
    vec3       saturation_color   = mix(intensity, brightness_color, saturation + 1.);
    vec3       contrast_color     = mix(average_luminosity, saturation_color, contrast + 1.);

    vec3 out_color = contrast_color;
    return mix(in_color, out_color, mask);
}