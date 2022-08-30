#version 410

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_line.glsl"

INPUT float Time_mod;
INPUT float Threshold; // default 0.7 min = 0 max = 2
// 0 forbidden
INPUT vec2 Distortion;
// 0 forbidden make crash
INPUT int Steps; // default 4 min 1 max 20

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = distortion_line(
        in_uv, Effect_intensity,
        Time_mod, Threshold, Distortion, Steps
    );

    vec3 out_color = image(out_uv);

    out_Color = vec4(out_color, 1.);
}