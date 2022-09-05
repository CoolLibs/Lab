#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/flare.glsl"

INPUT Direction2D Direction;
INPUT Point2D     Center;
INPUT float       Scale;
INPUT float       Brightness;        // 0 min -1. max 1.
INPUT int         Number_of_circles; // default 10
INPUT int         Seed;              // default 20

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv    = normalized_uv();
    vec3 in_color = image(in_uv);

    vec3 color = flare(
        in_color, Effect_intensity, in_uv,
        Center, Direction, Scale, Brightness, Number_of_circles, Seed
    );

    out_Color = vec4(color, 1.);
}