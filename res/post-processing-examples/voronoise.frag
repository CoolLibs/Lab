// Cellular noise ("Worley noise") in 2D in GLSL.
// Copyright (c) Stefan Gustavson 2011-04-19. All rights reserved.
// This code is released under the conditions of the MIT license.
// See LICENSE file for details.
// https://github.com/stegu/webgl-noise

#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/voronoise.glsl"

INPUT float Scale;

INPUT float Mask;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = voronoise_uv(
        in_uv, Mask,
        Scale
    );

    vec3 color = image(out_uv);

    // vec3 color = voronoise_color(
    //     in_uv,
    //     Scale
    // );

    out_Color = vec4(color, 1.);
}
