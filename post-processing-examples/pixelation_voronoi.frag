#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/pixelation_voronoi.glsl"

// BEGIN DYNAMIC PARAMS

uniform float Size;
uniform float Speed;
uniform float Movement;
uniform float Time_mode;
uniform float Distance_mode;

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    vec2 uv = _uv;
    uv.x *= _aspect_ratio;

    vec3 in_color = image(_uv);

    vec3 out_color = pixelation_voronoi(
        in_color, Effect_intensity, uv,
        Size, Speed, Movement, Time_mode, Distance_mode
    );

    out_Color = vec4(out_color, 1.);
}