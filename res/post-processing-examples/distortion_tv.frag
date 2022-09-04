#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_tv.glsl"

INPUT float Y_offset;
INPUT vec3  Channels_offset; // default vec3(20.,22.,14.)
INPUT int   Nb_iteration;    // min = 1 max 23

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec3 color = distortion_tv(
        in_uv, Effect_intensity,
        Channels_offset, Y_offset, Nb_iteration
    );

    out_Color = vec4(color, 1.0);
}