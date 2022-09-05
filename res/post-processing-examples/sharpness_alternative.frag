#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/sharpness_alternative.glsl"

INPUT float Offset_strength;
INPUT float Sharpening_strength;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec3 color = sharpness_alternative(
        in_uv, Effect_intensity,
        Offset_strength, Sharpening_strength
    );

    out_Color = vec4(color, 1.0);
}
