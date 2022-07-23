#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/vibrance.glsl"

input float Vibrance; // default 0.

input float Effect_intensity;

void main()
{
    RgbColor in_color = image(_uv);

    RgbColor out_color = vibrance(
        in_color, Effect_intensity,
        Vibrance
    );

    out_Color = vec4(out_color, 1.);
}