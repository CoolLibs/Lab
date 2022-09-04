#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_tv.glsl"

INPUT float Y_offset;
INPUT vec3  Channels_offset; // default vec3(20.,22.,14.)
INPUT int   Nb_iteration;    // min = 1 max 23

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec3 color = distortion_tv(
        in_uv, Effect_intensity,
        Channels_offset, Y_offset, Nb_iteration
    );

    out_Color = vec4(color, 1.0);
}