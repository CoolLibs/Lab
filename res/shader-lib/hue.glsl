// #include "_ROOT_FOLDER_/res/shader-lib/rgb_hsv_conversion.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/mix_hues.glsl"

vec3 Cool__hue(
    vec3 in_color, float effect_intensity,
    float hue
)
{
    vec3 image_hsl = rgb2hsv(in_color);
    image_hsl.x    = mix_hues(image_hsl.x, hue, effect_intensity);
    vec3 out_color = hsv2rgb(image_hsl);
    return out_color;
}