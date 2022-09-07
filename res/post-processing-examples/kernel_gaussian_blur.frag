#version 410

uniform float _time;

out vec4 out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/kernel_convolution.glsl"

INPUT float Spread;
INPUT float Sigma;

INPUT float Effect_intensity;

// https://en.wikipedia.org/wiki/Kernel_(image_processing)

const int nb                = 5;
const int kernel_size       = nb * 2 + 1;
const int kernel_array_size = kernel_size * kernel_size;

float[kernel_array_size] gaussian_blur()
{
    float[kernel_array_size] g_b;
    float sum  = 0;
    int   size = g_b.length();
    for (int i = 0; i < size; i++)
    {
        vec2 value = (index_2D(i, kernel_size) - kernel_size / 2);
        g_b[i]     = exp(-dot(value, value) / (2. * pow(Sigma, 2.)));
        sum += g_b[i];
    }
    for (int i = 0; i < size; i++)
    {
        g_b[i] /= sum;
    }
    return g_b;
}

void main()
{
    vec2 in_uv                                    = normalized_uv();
    float[kernel_array_size] kernel_gaussian_blur = gaussian_blur();
    CONVOLUTION(kernel_gaussian_blur, kernel_array_size, image, in_uv, Spread, Effect_intensity);
    out_Color = vec4(color, 1.);
}