// #include "_ROOT_FOLDER_/res/shader-lib/rgb_hsv_conversion.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"

vec3 desaturate(vec3 color, float amount)
{
    float luminance = cool__luminance(color);
    return mix(color, vec3(luminance), amount);
}

float shadows_detection(vec3 image)
{
    float luminance = rgb2hsv(image).z;
    return smoothstep(.9, 0.3, luminance);
}

float highlights_detection(vec3 image)
{
    float luminance = rgb2hsv(image).z;
    return smoothstep(.9, 1., luminance);
}

vec3 tint_shadows_and_highlights(
    vec3 in_color, float effect_intensity,
    vec3 color_shadows, vec3 color_highlights, float shadows_intensity, float highlights_intensity
)
{
    float shadows    = shadows_detection(in_color);
    float highlights = highlights_detection(in_color);

    vec3 out_color = mix(mix(in_color, color_highlights, highlights * highlights_intensity), color_shadows, shadows * shadows_intensity);

    return mix(in_color, out_color, effect_intensity);
}