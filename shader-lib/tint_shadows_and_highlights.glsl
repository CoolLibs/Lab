// #include "_ROOT_FOLDER_/shader-lib/rgb_hsv_conversion.glsl"
// #include "_ROOT_FOLDER_/shader-lib/luminance.glsl"

RgbColor desaturate(RgbColor color, float amount)
{
    float luminance = luminance(color);
    return mix(color, vec3(luminance), amount);
}

float shadows_detection(RgbColor image)
{
    float luminance = rgb2hsv(image).z;
    return smoothstep(.9, 0.3, luminance);
}

float highlights_detection(RgbColor image)
{
    float luminance = rgb2hsv(image).z;
    return smoothstep(.9, 1., luminance);
}

RgbColor tint_shadows_and_highlights(
    RgbColor in_color, float effect_intensity,
    RgbColor color_shadows, RgbColor color_highlights, float shadows_intensity, float highlights_intensity
)
{
    float shadows    = shadows_detection(in_color);
    float highlights = highlights_detection(in_color);

    RgbColor out_color = mix(mix(in_color, color_highlights, highlights * highlights_intensity), color_shadows, shadows * shadows_intensity);

    return mix(in_color, out_color, effect_intensity);
}