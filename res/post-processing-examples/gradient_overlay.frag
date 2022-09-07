#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/gradient_overlay.glsl"

INPUT Point2D Start_point;
INPUT Point2D End_point;

INPUT RgbColor Start_color;
INPUT RgbColor End_color;

INPUT float Strength;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv    = normalized_uv();
    vec3 in_color = image(in_uv);

    vec3 color = gradient_overlay(
        in_color, Effect_intensity, in_uv,
        Start_point, End_point, Start_color, End_color, Strength
    );

    out_Color = vec4(color, 1.);
}