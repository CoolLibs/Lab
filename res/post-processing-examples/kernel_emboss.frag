#version 410

uniform float _time;

out vec4 out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/kernel_convolution.glsl"

INPUT float Spread;

INPUT float Mask;

// https://en.wikipedia.org/wiki/Kernel_(image_processing)

#define emboss float[9]( \
    -2, -1, 0,           \
    -1, 1, 1,            \
    0, 1, 2              \
)

void main()
{
    vec2 in_uv = normalized_uv();
    CONVOLUTION(emboss, emboss.length(), image, in_uv, Spread, Mask);
    out_Color = vec4(color, 1.);
}