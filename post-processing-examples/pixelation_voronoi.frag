#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/pixelation_voronoi.glsl"

input float Size;
input float Speed;
input float Movement;
input bool  Time_mode;
input float Distance_mode;

input float Effect_intensity;

void main()
{
    vec2 uv = _uv;
    uv.x *= _aspect_ratio;

    RgbColor in_color = image(_uv);

    RgbColor out_color = pixelation_voronoi(
        in_color, Effect_intensity, uv,
        Size, Speed, Movement, Time_mode, Distance_mode
    );

    out_Color = vec4(out_color, 1.);
}