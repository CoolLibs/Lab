#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_tv.glsl"

INPUT float Time;
INPUT float Strength;
INPUT vec3  Channels_offset;
INPUT int   Iterations;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec3 color = vec3(
        image(
            in_uv + (terrain(
                         Effect_intensity,
                         in_uv.y * Channels_offset.x + Time,
                         Iterations,
                         Strength
                     ) /
                     200.)
        )
            .r,
        image(
            in_uv + (terrain(
                         Effect_intensity,
                         in_uv.y * Channels_offset.y + Time,
                         Iterations,
                         Strength
                     ) /
                     201.)
        )
            .g,
        image(
            in_uv + (terrain(
                         Effect_intensity,
                         in_uv.y * Channels_offset.z + Time,
                         Iterations,
                         Strength
                     ) /
                     202.)
        )
            .b
    );

    out_Color = vec4(color, 1.0);
}