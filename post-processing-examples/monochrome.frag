#version 410

layout(location = 0) in vec2 _uv;
out vec4 out_Color;

uniform vec3 _color;

void main()
{
    out_Color = vec4(_color, 1.);
}