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

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/classic_noise.glsl"

INPUT float Frequency;
INPUT float Scale;
INPUT float Fractalness;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = classic_noise_uv(
        in_uv, Effect_intensity,
        Frequency, Fractalness, Scale
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}
