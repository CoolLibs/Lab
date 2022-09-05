#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/flare.glsl"

INPUT Point2D Position;
INPUT float   Brightness; // 0 min -1. max 1.

INPUT int Number_of_circles; // default 10
INPUT int Seed;              // default 20

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec3 color = flare(
        in_uv, Effect_intensity,
        _aspect_ratio,
        Position, Brightness, Number_of_circles, Seed
    );

    out_Color = vec4(color, 1.);
}