#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/sharpen.glsl"

INPUT float Spread;
INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec3 color = sharpen(
        in_uv, Effect_intensity,
        Spread
    );

    out_Color = vec4(color, 1);
}