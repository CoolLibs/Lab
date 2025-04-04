#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/classic_noise.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_kaleidoscope_angular.glsl"

INPUT Angle   Angle_distortion;
INPUT float   Strength;
INPUT float   Strength2;
INPUT Point2D Center;

INPUT float Mask;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = distortion_kaleidoscope_angular(
        in_uv, Mask,
        Center, Angle_distortion, Strength, Strength2
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}