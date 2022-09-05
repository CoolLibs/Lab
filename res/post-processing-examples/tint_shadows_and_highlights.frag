#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/tint_shadows_and_highlights.glsl"

INPUT RgbColor Shadow_color;
INPUT float    Shadow_strength;
INPUT RgbColor Highlight_color;
INPUT float    Highlight_strength;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = tint_shadows_and_highlights(
        in_color, Effect_intensity,
        Shadow_color, Highlight_color, Shadow_strength, Highlight_strength
    );

    out_Color = vec4(color, 1.);
}