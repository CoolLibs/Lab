#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/color_inversion.glsl"

// default 1 min = 0
INPUT vec3 Channels_contribution;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = color_inversion(
        in_color, Effect_intensity,
        Channels_contribution
    );

    out_Color = vec4(color, 1.);
}