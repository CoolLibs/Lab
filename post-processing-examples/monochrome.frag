#version 430

layout(location = 0) in vec2 _uv;
out vec4 out_Color;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"

uniform RgbColor _color;

void main()
{
    out_Color = vec4(_color, 1.);
}