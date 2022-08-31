#version 410

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/raindrops.glsl"

INPUT float Scale; // 0 forbidden
INPUT float Blur;  //
INPUT float Time_mod;
INPUT float Rain_amount; // between 0 and 1

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec3 out_color = raindrops_uv(
        in_uv, Effect_intensity,
        _aspect_ratio,
        Time_mod, Scale, Rain_amount, Blur
    );

    out_Color = vec4(out_color, 1.);
}