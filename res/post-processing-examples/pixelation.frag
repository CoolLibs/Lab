#version 410

//#include "_COOL_RES_/shaders/math.glsl"

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/pixelation.glsl"

INPUT int   Nb_tiles_on_y_axis;
INPUT float Border_thickness;
INPUT float Border_roundness;

INPUT RgbColor Border_color;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec3 color = pixelation_with_border(
        in_uv, Effect_intensity,
        Nb_tiles_on_y_axis, 1. - Border_thickness, Border_roundness, Border_color
    );

    out_Color = vec4(color, 1.);
}