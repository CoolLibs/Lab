#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"

INPUT Gradient gradient;

void main()
{
    out_Color = gradient(_uv.x);
}
