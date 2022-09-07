#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/wormhole.glsl"

INPUT float   Attractivity; // default 0.008
INPUT float   Subdivisions; // min = 0 max = 0.5
INPUT Point2D Center;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = wormhole(
        in_uv, Effect_intensity,
        Center, Attractivity, Subdivisions
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}
