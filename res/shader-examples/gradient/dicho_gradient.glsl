// #include "_ROOT_FOLDER_/shader-examples/gradient/dicho.glsl"

vec4 constant_dicho(Mark gradient[number_of_marks], float uv_x)
{
    if (number_of_marks == 1)
    {
        return gradient[0].col;
    }
    else
    {
        if (uv_x > gradient[number_of_marks - 1].pos)
        {
            return gradient[number_of_marks - 1].col;
        }
        else
        {
            int index = dicho(gradient, uv_x);
            if (uv_x > gradient[index].pos)
            {
                return gradient[index].col;
            }
            else
            {
                if (index == 0)
                {
                    return gradient[0].col;
                }
                else
                {
                    return gradient[index - 1].col;
                }
            }
        }
    }
}

vec4 linear_dicho(Mark gradient[number_of_marks], float uv_x)
{
    if (number_of_marks == 1)
    {
        return gradient[0].col;
    }

    else
    {
        if (uv_x > gradient[number_of_marks - 1].pos)
        {
            return gradient[number_of_marks - 1].col;
        }
        if (uv_x < gradient[0].pos)
        {
            return gradient[0].col;
        }
        else
        {
            int index = dicho(gradient, uv_x);
            if ((uv_x <= gradient[index].pos))
            {
                float mix_factor = (uv_x - gradient[index - 1].pos) /
                                   (gradient[index].pos - gradient[index - 1].pos);
                return mix(gradient[index - 1].col, gradient[index].col, mix_factor);
            }
            else
            {
                float mix_factor = (uv_x - gradient[index].pos) /
                                   (gradient[index + 1].pos - gradient[index].pos);
                return mix(gradient[index].col, gradient[index + 1].col, mix_factor);
            }
        }
    }
}