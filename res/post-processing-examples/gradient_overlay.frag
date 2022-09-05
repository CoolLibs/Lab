#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/gradient_overlay.glsl"

INPUT vec2 Gradient_pos_a;
INPUT vec2 Gradient_pos_b;

INPUT RgbColor Gradient_color_a;
INPUT RgbColor Gradient_color_b;

INPUT float Gradient_presence;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv    = normalized_uv();
    vec3 in_color = image(in_uv);

    vec3 color = gradient_overlay(
        in_color, Effect_intensity, in_uv,
        Gradient_pos_a, Gradient_pos_b, Gradient_color_a, Gradient_color_b, Gradient_presence
    );

    out_Color = vec4(color, 1.);
}