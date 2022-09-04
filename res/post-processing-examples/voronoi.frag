#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/voronoi.glsl"

INPUT float Scale; // 10
INPUT float Speed;
INPUT float Movement;
INPUT bool  Time_mode;
INPUT float Distance_mode; // 2

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = voronoi_uv(
        in_uv, Effect_intensity,
        Scale, Speed, Movement, Time_mode, Distance_mode
    );

    vec3 color = image(out_uv);

    // vec3 color = voronoi_color(
    //     in_uv,
    //     Scale, Speed, Movement, Time_mode, Distance_mode
    // );

    out_Color = vec4(color, 1.);
}