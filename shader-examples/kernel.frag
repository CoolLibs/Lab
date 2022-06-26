#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;
uniform sampler2D _texture;

// #include "_ROOT_FOLDER_/shader-lib/kernel.glsl"

// BEGIN DYNAMIC PARAMS

const int nb          = 1;
const int kernel_size = nb * 2 + 1;

uniform float space_between_two_pixels_times_1000;
uniform float sigma;
uniform float normaliza;

// END DYNAMIC PARAMS

void main()
{
    vec2 uv = _uv;
    uv.x *= _aspect_ratio;

    vec3 out_color = kernel(uv, nb, kernel_size, space_between_two_pixels_times_1000, sigma, normaliza);

    out_Color = vec4(color, 1.);
}