// https://www.shadertoy.com/view/fddXRf

#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"

INPUT ColorPalette Palette;
INPUT float        Color_offset;

INPUT float Mask;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = Palette(0);

    for (int i = 0; i < Palette_length; i++)
    {
        float ii  = float(i);
        float cut = smoothstep(Color_offset * ii, Color_offset * (ii + 1.), cool__luminance(in_color));
        color     = mix(color, Palette(i), cut);
    }
    color = mix(in_color, color, Mask);

    out_Color = vec4(color, 1.);
}