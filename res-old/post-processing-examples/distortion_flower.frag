#version 410

// #include "_COOL_RES_/shaders/math.glsl"

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_flower.glsl"

INPUT Point2D Center;
INPUT float   Scale;
INPUT float   Frequency;
INPUT float   Wave_compression;
INPUT int     Subdivison;

INPUT float Mask;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = distortion_flower(
        in_uv, Mask,
        Center, Scale, Frequency, Wave_compression, float(Subdivison)
    );

    vec3 out_color = image(out_uv);

    out_Color = vec4(out_color, 1.0);
}