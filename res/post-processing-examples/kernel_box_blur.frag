#version 410

uniform float _time;

out vec4 out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/kernel_convolution.glsl"

INPUT float Spread;

INPUT float Effect_intensity;

// https://en.wikipedia.org/wiki/Kernel_(image_processing)

const int nb                = 1;
const int kernel_size       = nb * 2 + 1;
const int kernel_array_size = kernel_size * kernel_size;

float[kernel_array_size] box_blur()
{
    float[kernel_array_size] box_blur_kernel;
    int size = box_blur_kernel.length();
    for (int i = 0; i < size; i++)
    {
        box_blur_kernel[i] = 1. / size;
    }
    return box_blur_kernel;
}

void main()
{
    vec2 in_uv                               = normalized_uv();
    float[kernel_array_size] kernel_box_blur = bokeh_blur();
    CONVOLUTION(kernel_box_blur, kernel_box_blur.length(), image, in_uv, Spread, Effect_intensity);
    out_Color = vec4(color, 1.);
}