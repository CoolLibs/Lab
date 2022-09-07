#version 410

// #include "_COOL_RES_/shaders/math.glsl"

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_simple.glsl"

INPUT Point2D Center;
INPUT float   Strength;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = distortion_simple(
        in_uv, Effect_intensity,
        Center, Strength
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}