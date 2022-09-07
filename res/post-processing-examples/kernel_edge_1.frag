#version 410

uniform float _time;

out vec4 out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/kernel_convolution.glsl"

INPUT float Spread;

INPUT float Effect_intensity;

// https://en.wikipedia.org/wiki/Kernel_(image_processing)

#define edge1 float[9]( \
    +0, +1, +0,         \
    +1, -4, +1,         \
    +0, +1, +0          \
)

void main()
{
    vec2 in_uv = normalized_uv();
    CONVOLUTION(edge1, edge1.length(), image, in_uv, Spread, Effect_intensity);
    out_Color = vec4(color, 1.);
}