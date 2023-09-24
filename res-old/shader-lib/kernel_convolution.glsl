// #include "_ROOT_FOLDER_/res/shader-lib/kernel_neighbour_offset_in_image_space.glsl"

// Convolve a texture with kernel
// kernel : kernel used for convolution
// uv : current coordinates on sampler
#define CONVOLUTION(KERNEL, SIZE, IMAGE, UV, SPACE_BETWEEN_PIXELS, EFFECT_INTENSITY)                                    \
    vec3 color = vec3(0.);                                                                                              \
    for (int i = 0; i < SIZE; i++)                                                                                      \
    {                                                                                                                   \
        vec3  neighbour_color  = IMAGE(UV + neighbour_offset_in_image_space(i, KERNEL.length(), SPACE_BETWEEN_PIXELS)); \
        float weight           = i != SIZE / 2                                                                          \
                                     ? KERNEL[i] * EFFECT_INTENSITY                                                     \
                                     : mix(1., KERNEL[i], EFFECT_INTENSITY);                                            \
        vec3  neighbour_kernel = weight * neighbour_color;                                                              \
        color += neighbour_kernel;                                                                                      \
    }
