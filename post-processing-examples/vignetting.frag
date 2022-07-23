#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/vignetting.glsl"

input float Border_darkness; // Default 0.262
input float Center_radius;   // default 21.

input float Effect_intensity;

void main()
{
    vec2 uv = _uv;
    uv *= 1.0 - uv.yx;
    uv.x *= _aspect_ratio;

    RgbColor in_color = image(_uv);

    RgbColor out_color = vignetting(
        in_color, Effect_intensity, uv,
        Border_darkness, Center_radius
    );

    out_Color = vec4(out_color, 1.);
}