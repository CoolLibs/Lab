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

#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/classic_noise.glsl"

INPUT float Frequency; // 0.5 min 0.001 max 10.

INPUT float Scale; // 100. min 0. max 200.

INPUT float Fractalness; // 1.5 min 1. max 5.

INPUT float Effect_intensity; // default 1. min -1. max 2.

void main()
{
    vec2 uv = _uv;

    vec2 out_uv = classic_noise_uv(
        uv, Effect_intensity,
        Frequency, Fractalness, Scale
    );

    out_Color = vec4(image(out_uv), 1.);
}
