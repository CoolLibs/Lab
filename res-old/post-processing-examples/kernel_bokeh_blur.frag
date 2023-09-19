#version 410

uniform float _time;

out vec4 out_Color;

uniform sampler2D _texture;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/kernel_convolution.glsl"

INPUT float Spread;
INPUT float normaliza;

INPUT float Mask;

// https://en.wikipedia.org/wiki/Kernel_(image_processing)

const int nb                = 1;
const int kernel_size       = nb * 2 + 1;
const int kernel_array_size = kernel_size * kernel_size;

float[kernel_array_size] bokeh_blur()
{
    float[kernel_array_size] b_b;
    float sum  = 0;
    int   size = b_b.length();
    for (int i = 0; i < size; i++)
    {
        vec2 index = index_2D(i, kernel_size) / float(kernel_size - 1);
        index.y    = 1. - index.y;
        b_b[i]     = texture(_texture, index).r;
        sum += b_b[i];
    }
    for (int i = 0; i < size; i++)
    {
        b_b[i] /= pow(sum, normaliza);
    }
    return b_b;
}

void main()
{
    vec2 in_uv                                 = normalized_uv();
    float[kernel_array_size] kernel_bokeh_blur = bokeh_blur();
    CONVOLUTION(kernel_bokeh_blur, kernel_array_size, image, in_uv, Spread, Mask);
    out_Color = vec4(color, 1.);
}