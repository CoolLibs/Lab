#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/flare.glsl"

INPUT Point2D Position;   // vec2(0.5,0.5)
INPUT float   Brightness; // 0 min -1. max 1.

INPUT int Number_of_circles; // default 10
INPUT int Seed;              // default 20

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec3 out_color = flare(
        in_uv, Effect_intensity,
        _aspect_ratio,
        Position, Brightness, Number_of_circles, Seed
    );

    out_Color = vec4(out_color, 1.);
}