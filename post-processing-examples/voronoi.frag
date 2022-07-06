#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/voronoi.glsl"

// BEGIN DYNAMIC PARAMS

uniform float Scale; // 10
uniform float Speed;
uniform float Movement;
uniform bool  Time_mode;
uniform float Distance_mode; // 2

uniform float Effect_intensity;

// END DYNAMIC PARAMS

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