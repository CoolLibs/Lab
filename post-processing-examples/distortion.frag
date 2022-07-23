#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;
uniform sampler2D _texture;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/distortion.glsl"

input float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;
    in_uv.x *= _aspect_ratio;

    vec2 out_uv = distortion(in_uv, Effect_intensity);

    out_uv.x /= _aspect_ratio;
    RgbColor out_color = image(out_uv);

    out_Color = vec4(out_color, 1.);
}