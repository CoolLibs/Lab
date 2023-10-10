#version 410

uniform float _time;

out vec4 out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/kernel_convolution.glsl"

INPUT float Spread;

INPUT float Mask;

// https://en.wikipedia.org/wiki/Kernel_(image_processing)

#define edge2 float[9]( \
    -1, -1, -1,         \
    -1, +8, -1,         \
    -1, -1, -1          \
)

void main()
{
    vec2 in_uv = normalized_uv();
    CONVOLUTION(edge2, edge2.length(), image, in_uv, Spread, Mask);
    out_Color = vec4(color, 1.);
}