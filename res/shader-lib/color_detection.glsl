// #include "_ROOT_FOLDER_/res/shader-lib/rgb_hsv_conversion.frag"

float color_detection(
    vec3 in_color,
    vec3 color
)
{
    float hue_color = rgb2hsv(color).x;
    float hue_pixel = rgb2hsv(in_color).x;
    return smoothstep(.2, .01, min(abs(hue_color - hue_pixel), abs(hue_color + 1 - hue_pixel)));
}