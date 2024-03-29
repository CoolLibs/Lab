#version 410

// #include "_COOL_RES_/shaders/math.glsl"

uniform float _time;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/raindrops.glsl"

INPUT float Scale; // 0 forbidden
INPUT float Blur;  //
INPUT float Time_mod;
INPUT float Rain_amount; // between 0 and 1

INPUT float Mask;

void main()
{
    vec2 in_uv = normalized_uv();

    vec3 color = raindrops_uv(
        in_uv, Mask, _image,
        Time_mod, Scale, Rain_amount, Blur
    );

    out_Color = vec4(color, 1.);
}