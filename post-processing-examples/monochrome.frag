#version 430

layout(location = 0) in vec2 _uv;
out vec4 out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"

input RgbColor _color;

void main()
{
    out_Color = vec4(_color, 1.);
}