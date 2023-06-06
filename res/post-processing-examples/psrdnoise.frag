#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/fbm.glsl"

INPUT float Scale;
INPUT float Gain;
INPUT float Lacunarity;
INPUT int   Octaves;

INPUT float Mask;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = fbm(
        in_uv, Mask,
        Scale, Lacunarity, Gain, Octaves
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}