// #include "_ROOT_FOLDER_/res/shader-examples/gradient/Mark.glsl"

vec4 constant(Mark gradient[number_of_marks_test], float uv_x)
{
    if (uv_x <= gradient[0].pos)
    {
        return gradient[0].col;
    }
    for (int i = 1; i < number_of_marks_test; i++)
    {
        if ((uv_x <= gradient[i].pos) && (uv_x >= gradient[i - 1].pos))
        {
            return gradient[i - 1].col;
        }
    }
    if (uv_x >= gradient[number_of_marks_test - 1].pos)
    {
        return gradient[number_of_marks_test - 1].col;
    }
}