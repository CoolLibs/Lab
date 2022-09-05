#version 410

// #include "_COOL_RES_/shaders/math.glsl"

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/swirl_distortion.glsl"

INPUT Point2D Center;
INPUT float   Size; // only positive values
INPUT float   Strength;

INPUT float Effect_intensity; // positive and negative values

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = swirl_distortion(
        in_uv, Effect_intensity,
        Center, Strength, Size
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}