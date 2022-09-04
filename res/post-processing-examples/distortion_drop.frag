#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_drop.glsl"

INPUT float   Drops_size;           // default 0.2 min = .001 max = 1.5
INPUT float   Distortion_intensity; // default 0.7 min = 0 max = 1
INPUT float   Scale;                // default 1 min = 0.001 max = 1.5
INPUT float   Time;
INPUT Point2D Center;
INPUT int     N;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = distortion_drop(
        in_uv, Effect_intensity,
        Center, Drops_size, Scale, Distortion_intensity, Time, N
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1);
}