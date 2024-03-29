#version 410

// #include "_COOL_RES_/shaders/math.glsl"

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/pixelation.glsl"

INPUT int   Scale;
INPUT float Border_thickness;
INPUT float Border_roundness;

INPUT RgbColor Border_color;

INPUT float Mask;

void main()
{
    vec2 in_uv = normalized_uv();

    vec3 color = pixelation_with_border(
        in_uv, Mask,
        Scale, 1. - Border_thickness, Border_roundness, Border_color
    );

    out_Color = vec4(color, 1.);
}