#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/glitch.glsl"

INPUT float Time;
INPUT vec2  Squares_size; // default vec2(24., 9.)

INPUT float Effect_intensity; // default .05 min 0. max 1.

void main()
{
    vec2 in_uv = _uv;

    vec3 color = glitch(
        in_uv, Effect_intensity,
        Squares_size, Time
    );

    out_Color = vec4(color, 1.);
}
