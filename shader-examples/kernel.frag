#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

const int nb          = 10;
const int kernel_size = nb * 2 + 1;

uniform float space_between_two_pixels_times_1000;
uniform float sigma;

// END DYNAMIC PARAMS

// https://en.wikipedia.org/wiki/Kernel_(image_processing)

// Define kernels
#define identity float[9](0, 0, 0, \
                          0, 1, 0, \
                          0, 0, 0)
#define edge0 float[9](+1, +0, -1, \
                       +0, +0, +0, \
                       -1, +0, +1)
#define edge1 float[9](+0, +1, +0, \
                       +1, -4, +1, \
                       +0, +1, +0)
#define edge2 float[9](-1, -1, -1, \
                       -1, +8, -1, \
                       -1, -1, -1)
#define sharpen float[9](0, -1, 0, -1, 5, -1, 0, -1, 0)
// #define box_blur      float[9](1, 1, 1, 1, 1, 1, 1, 1, 1) * 0.1111
// #define gaussian_blur float[9](1, 2, 1, 2, 4, 2, 1, 2, 1) * 0.0625
#define emboss float[9](-2, -1, 0, -1, 1, 1, 0, 1, 2)

#define PI 3.1415926538

float[kernel_size * kernel_size] box_blur()
{
    return float[kernel_size * kernel_size](1. / (kernel_size * kernel_size));
}

ivec2 index_2D(int index_1D)
{
    return ivec2(index_1D % kernel_size,
                 index_1D / kernel_size);
}

float[kernel_size * kernel_size] gaussian_blur()
{
    float[kernel_size * kernel_size] g_b;
    float sum = 0;
    for (int i = 0; i < kernel_size * kernel_size; i++) {
        vec2 value = (index_2D(i) - kernel_size / 2); //* space_between_two_pixels_times_1000 / 1000.f;
        g_b[i]     = exp(-dot(value, value) /
                         (2. * pow(sigma, 2.)));
        sum += g_b[i];
    }
    for (int i = 0; i < kernel_size * kernel_size; i++) {
        g_b[i] /= sum;
    }
    return g_b;
}

vec2 neighbour_offset_in_image_space(int index)
{
    return (index_2D(index) - kernel_size / 2) * space_between_two_pixels_times_1000 / 1000.f;
}

// Convolve a texture with kernel
// kernel : kernel used for convolution
// sampler : texture sampler
// uv : current coordinates on sampler
vec3 convolution(float kernel[kernel_size * kernel_size], sampler2D sampler, vec2 uv)
{
    vec3 sum = vec3(0.);
    for (int i = 0; i < kernel_size * kernel_size; i++) {
        vec3 neighbour_color  = texture(sampler, uv + neighbour_offset_in_image_space(i)).rgb;
        vec3 neighbour_kernel = kernel[i] * neighbour_color;
        sum += neighbour_kernel;
    }

    return sum;
}

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec3 color = convolution(gaussian_blur(), _image, _uv);
    out_Color  = vec4(color, 1.);
}