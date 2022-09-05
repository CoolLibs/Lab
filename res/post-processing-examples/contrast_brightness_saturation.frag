#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/contrast_brightness_saturation.glsl"

INPUT float Contrast;   // default 0.0
INPUT float Brightness; // default 0.0
INPUT float Saturation; // default 0.0

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