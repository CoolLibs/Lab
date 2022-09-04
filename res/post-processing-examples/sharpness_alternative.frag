#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/sharpness_alternative.glsl"

INPUT float Offset_strength;
INPUT float Sharpening_strength;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec3 color = sharpness_alternative(
        in_uv, Effect_intensity,
        Offset_strength, Sharpening_strength
    );

    out_Color = vec4(color, 1.0);
}
