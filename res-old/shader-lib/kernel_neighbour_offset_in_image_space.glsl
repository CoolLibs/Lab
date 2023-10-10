// #include "_ROOT_FOLDER_/res/shader-lib/kernel_index_2D.glsl"

vec2 neighbour_offset_in_image_space(int index, int kernel_size, float space_between_two_pixels_times_1000)
{
    return (index_2D(index, kernel_size) - kernel_size / 2) * space_between_two_pixels_times_1000 / 1000.f;
}