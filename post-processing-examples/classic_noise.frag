//
// GLSL textureless classic 2D noise "cnoise",
// with an RSL-style periodic variant "pnoise".
// Author:  Stefan Gustavson (stefan.gustavson@liu.se)
// Version: 2011-08-22
//
// Many thanks to Ian McEwan of Ashima Arts for the
// ideas for permutation and gradient selection.
//
// Copyright (c) 2011 Stefan Gustavson. All rights reserved.
// Distributed under the MIT license. See LICENSE file.
// https://github.com/stegu/webgl-noise

#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/classic_noise.glsl"

// BEGIN DYNAMIC PARAMS

uniform float Frequency;

uniform float Scale; // 100

uniform float Fractalness; // min = 1, max = 3; default = 1.5;   increases the fractalness of the noise

uniform float Effect_intensity; // default 1

// END DYNAMIC PARAMS

void main()
{
    vec2 uv = _uv;

    vec2 uv_noise = classic_noise_uv(
        Effect_intensity, uv,
        Frequency, Fractalness, Scale
    );

    vec3 out_color = image(uv_noise);

    out_Color = vec4(out_color, 1.);
}
