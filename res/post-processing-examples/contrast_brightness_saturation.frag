#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/contrast_brightness_saturation.glsl"

INPUT float Contrast;
INPUT float Brightness;
INPUT float Saturation;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = contrast_brightness_saturation(
        in_color, Effect_intensity,
        Contrast, Brightness, Saturation
    );

    out_Color = vec4(color, 1.);
}