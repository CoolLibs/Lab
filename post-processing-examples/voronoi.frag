#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/voronoi.glsl"

input float Scale; // 10
input float Speed;
input float Movement;
input bool  Time_mode;
input float Distance_mode; // 2

input float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;
    in_uv.x *= _aspect_ratio;

    RgbColor in_color = image(_uv);

    RgbColor out_color = voronoi(
        in_color, Effect_intensity, in_uv,
        Scale, Speed, Movement, Time_mode, Distance_mode
    );

    out_Color = vec4(out_color, 1.);
}