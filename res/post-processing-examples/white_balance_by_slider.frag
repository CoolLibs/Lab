// Color temperature (sRGB) stuff
// Copyright (C) 2014 by Benjamin 'BeRo' Rosseaux
// Because the german law knows no public domain in the usual sense,
// this code is licensed under the CC0 license
// http://creativecommons.org/publicdomain/zero/1.0/

#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/white_balance_by_slider.glsl"

INPUT float Temperature; // default 1000 to 40000
INPUT float Temperature_strength;
INPUT float Luminance_preservation_factor;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(_uv).xyz;

    vec3 color = white_balance_by_slider(
        in_color, Effect_intensity,
        Temperature, Temperature_strength, Luminance_preservation_factor
    );

    out_Color = vec4(color, 1.0);
}
