#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/shadows_highlights.glsl"

INPUT float factor_shadows;
INPUT float power_shadows;
INPUT float dir_shadows;
INPUT float factor_highlights;
INPUT float power_highlights;
INPUT float dir_highlights;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = shadows_highlights(
        in_color, Effect_intensity,
        factor_shadows, power_shadows, dir_shadows,
        factor_highlights, power_highlights, dir_highlights
    );

    out_Color = vec4(color, 1.);
}