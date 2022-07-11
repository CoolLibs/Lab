// Cellular noise ("Worley noise") in 2D in GLSL.
// Copyright (c) Stefan Gustavson 2011-04-19. All rights reserved.
// This code is released under the conditions of the MIT license.
// See LICENSE file for details.
// https://github.com/stegu/webgl-noise

#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/voronoise.glsl"

// BEGIN DYNAMIC PARAMS

uniform float Scale;

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    vec2 in_uv = _uv;

    vec3 in_color = image(_uv);

    vec2 uv_voronoise = voronoise(
        Effect_intensity, in_uv,
        Scale
    );

    vec3 out_color = image(uv_voronoise);

    out_Color = vec4(out_color, 1.);
}