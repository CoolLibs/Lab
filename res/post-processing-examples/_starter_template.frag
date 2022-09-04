#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

void main()
{
    out_Color = vec4(image(_uv).rgr, 1.);
}