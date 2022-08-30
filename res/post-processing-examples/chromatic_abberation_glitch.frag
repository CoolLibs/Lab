#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/chromatic_aberration_glitch.glsl"

INPUT float Time_mod;
INPUT float Tint;     // default .5 min -1. max 1.
INPUT float Darkness; // default .08 min -1. max 1.
INPUT vec2  BlockS;   // default vec2(24., 9.)
INPUT vec2  BlockL;   // default vec2(8., 4.)

INPUT float Effect_intensity; // default .05 min 0. max 1.

void main()
{
    vec2 in_uv = _uv;

    vec3 out_color = chromatic_aberration_glitch(
        in_uv, Effect_intensity,
        Tint, Darkness, BlockS, BlockL, Time_mod
    );

    out_Color = vec4(out_color, 1.);
}
