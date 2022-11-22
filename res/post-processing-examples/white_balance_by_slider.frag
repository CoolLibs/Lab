// Color temperature (sRGB) stuff
// Copyright (C) 2014 by Benjamin 'BeRo' Rosseaux
// Because the german law knows no public domain in the usual sense,
// this code is licensed under the CC0 license
// http://creativecommons.org/publicdomain/zero/1.0/

#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/white_balance_by_slider.glsl"

INPUT float Temperature;
INPUT float Temperature_strength;
INPUT float Luminance_preservation_factor;

INPUT float Mask;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = white_balance_by_slider(
        in_color, Mask,
        Temperature, Temperature_strength, Luminance_preservation_factor
    );

    out_Color = vec4(color, 1.0);
}
