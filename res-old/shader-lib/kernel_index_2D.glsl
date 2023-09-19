ivec2 index_2D(int index_1D, int kernel_size)
{
    return ivec2(index_1D % kernel_size, index_1D / kernel_size);
}