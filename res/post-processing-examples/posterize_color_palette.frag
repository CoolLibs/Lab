#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/threshold.glsl"

INPUT ColorPalette Palette;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(normalized_uv());

    float luminance = cool__luminance(in_color);
    luminance       = threshold(luminance, Palette_length);

    vec3 color = Palette(int(luminance * (Palette_length - 1.)));

    color = mix(in_color, color, Effect_intensity);

    out_Color = vec4(color, 1.);
}