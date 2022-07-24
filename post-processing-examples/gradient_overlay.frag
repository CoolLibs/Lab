#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/gradient_overlay.glsl"

INPUT vec2 Gradient_pos_a;
INPUT vec2 Gradient_pos_b;

INPUT RgbColor Gradient_color_a;
INPUT RgbColor Gradient_color_b;

INPUT float Gradient_presence;

INPUT bool Use_the_best_blending_technique;

INPUT float Effect_intensity;

void main()
{
    RgbColor in_color = image(_uv);

    RgbColor out_color = gradient_overlay(
        in_color, Effect_intensity,
        Gradient_pos_a, Gradient_pos_b, Gradient_color_a, Gradient_color_b, Gradient_presence, Use_the_best_blending_technique
    );

    out_Color = vec4(out_color, 1.);
}