// #include "_ROOT_FOLDER_/shader-examples/gradient/Mark.glsl"

int dicho(Mark gradient[number_of_marks], float uv_x)
{
    int start  = 0;
    int end    = number_of_marks - 1;
    int middle = 0;
    while (end - start > 0)
    {
        middle = (start + end) % 2 == 0
                     ? (start + end) / 2
                     : (start + end - 1) / 2;
        if (gradient[middle].pos >= uv_x)
        {
            end = middle;
        }
        else
        {
            start = middle + 1;
        }
    }
    return middle;
}