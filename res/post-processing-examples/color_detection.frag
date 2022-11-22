#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/color_detection.glsl"

INPUT RgbColor Color;
INPUT float    Saturation;

INPUT float Mask;

void main()
{
    vec3  in_color  = image(normalized_uv());
    float hue       = color_detection(in_color, Color);
    vec3  image_hsl = rgb2hsv(in_color);
    image_hsl += vec3(0., Saturation * hue, 0.);
    vec3 color = hsv2rgb(image_hsl);
    out_Color  = vec4(color, 1.);
}