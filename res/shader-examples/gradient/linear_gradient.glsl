// #include "_COOL_RES_/shaders/GradientMark.glsl"

vec4 linear(GradientMark gradient[number_of_marks_test], float uv_x)
{
    if (uv_x <= gradient[0].pos)
    {
        return gradient[0].col;
    }
    for (int i = 1; i < number_of_marks_test; i++)
    {
        if ((uv_x <= gradient[i].pos) && (uv_x >= gradient[i - 1].pos))
        {
            float mix_factor = (uv_x - gradient[i - 1].pos) /
                               (gradient[i].pos - gradient[i - 1].pos);
            return mix(gradient[i - 1].col, gradient[i].col, mix_factor);
        }
    }
    if (uv_x >= gradient[number_of_marks_test - 1].pos)
    {
        return gradient[number_of_marks_test - 1].col;
    }
}