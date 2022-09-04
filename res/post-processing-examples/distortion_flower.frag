#version 410

// #include "_COOL_RES_/shaders/math.glsl"

uniform float _time;
out vec4      out_Color;

// TODO(ASG) Fix displacement when increasig scale at low effect_intensity

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_flower.glsl"

// default 0.5 range 0 to 1
// Point2D
INPUT vec2  Center;
INPUT float Scale;
INPUT float Frequency;
INPUT int   Subdivison;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = distortion_flower(
        in_uv, Effect_intensity,
        Center, Scale, Frequency, Subdivison
    );

    vec3 out_color = image(out_uv);

    out_Color = vec4(out_color, 1.0);
}