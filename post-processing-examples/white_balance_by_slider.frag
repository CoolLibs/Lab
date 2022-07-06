// Color temperature (sRGB) stuff
// Copyright (C) 2014 by Benjamin 'BeRo' Rosseaux
// Because the german law knows no public domain in the usual sense,
// this code is licensed under the CC0 license
// http://creativecommons.org/publicdomain/zero/1.0/

#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"
// #include "_ROOT_FOLDER_/shader-lib/luminance.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/white_balance_by_slider.glsl"

// BEGIN DYNAMIC PARAMS

uniform float Temperature; // default 1000 to 40000
uniform float Temperature_strength;
uniform float Luminance_preservation_factor;

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    RgbColor in_color = image(_uv).xyz;

    RgbColor out_color = white_balance_by_slider(
        in_color, Effect_intensity,
        Temperature, Temperature_strength, Luminance_preservation_factor
    );

    out_Color = vec4(out_color, 1.0);
}