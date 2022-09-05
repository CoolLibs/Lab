#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/threshold.glsl"

INPUT int      Number_of_colors; // min 2
INPUT Gradient gradient;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(normalized_uv());

    float luminance = luminance(in_color);
    luminance       = threshold(luminance, Number_of_colors);

    vec3 color = gradient(luminance).rgb;

    color = mix(in_color, color, Effect_intensity);

    out_Color = vec4(color, 1.);
}