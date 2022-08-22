#version 410

// https://www.shadertoy.com/view/tdXfW7

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

struct Mark {
    float pos;
    vec4  col;
};

#define TEST_IDX 2
#define METHOD   4 // 1 : constant, 2 : linear, 3 : constant dicho, 4 : linear dicho

const int number_of_marks = TEST_IDX;
const int benchmark       = 2000;

#if TEST_IDX == 0

Mark gradient_data[];

#elif TEST_IDX == 1

Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(1, 0, 0, 1))
);

#elif TEST_IDX == 2

Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, .1, 1))

);

#elif TEST_IDX == 3
Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(1, 0, 1, 1)),
    Mark(0.3f, vec4(0, .1, 1, 1))
);

#elif TEST_IDX == 4
Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.3f, vec4(0, .5, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1))
);
#elif TEST_IDX == 5
Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.35f, vec4(0, .1, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.55f, vec4(0.3, 1, 0, 1))
);
#elif TEST_IDX == 6
Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.3f, vec4(0, .1, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.55f, vec4(0.3, 1, 0, 1)),
    Mark(0.75f, vec4(0.5, 1, 0, 1))
);
#elif TEST_IDX == 7

Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.3f, vec4(0, .1, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.55f, vec4(0.3, 1, 0, 1)),
    Mark(0.75f, vec4(0.5, 1, 0, 1)),
    Mark(0.8f, vec4(1, 1, 0, 1))
);
#elif TEST_IDX == 8
Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.3f, vec4(0, .1, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.55f, vec4(0.3, 1, 0, 1)),
    Mark(0.75f, vec4(0.5, 1, 0, 1)),
    Mark(0.8f, vec4(1, 1, 0, 1)),
    Mark(0.85f, vec4(1, 0.25, 0.5, 1))
);
#elif TEST_IDX == 9

Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.3f, vec4(0, .1, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.55f, vec4(0.3, 0, 0, 1)),
    Mark(0.75f, vec4(0.5, 1, 0, 1)),
    Mark(0.8f, vec4(1, 1, 0, 1)),
    Mark(0.85f, vec4(1, 0.25, 0.5, 1)),
    Mark(0.95f, vec4(1, 0, 0.8, 1))
);

#endif

#if TEST_IDX == 0

// glsl does not accept to have an array with a size of 0,
// therefore we have to create a particular case
void main()
{
    for (int i = 0; i < benchmark; ++i)
    {
        vec4 color = vec4(0);
        out_Color  = color;
    }
}

#else

vec4 constant(Mark gradient[number_of_marks], float uv_x)
{
    if ((uv_x <= gradient[0].pos) && (uv_x >= 0.f))
    {
        return gradient[0].col;
    }
    for (int i = 1; i < number_of_marks; i++)
    {
        if ((uv_x <= gradient[i].pos) && (uv_x >= gradient[i - 1].pos))
        {
            return gradient[i - 1].col;
        }
    }
    if ((uv_x <= 1.f) && (uv_x >= gradient[number_of_marks - 1].pos))
    {
        return gradient[number_of_marks - 1].col;
    }
}

int dicho(Mark gradient[number_of_marks], float uv_x)
{
    int start  = 0;
    int end    = number_of_marks - 1;
    int middle = 0;
    while (end - start > 0)
    {
        middle = (start + end) % 2 == 0 ? (start + end) / 2 : (start + end - 1) / 2;
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

// TODO(ASG) benchmark to see which method is fastest

vec4 linear(Mark gradient[number_of_marks], float uv_x)
{
    if ((uv_x <= gradient[0].pos) && (uv_x >= 0.f))
    {
        return gradient[0].col;
    }
    for (int i = 1; i < number_of_marks; i++)
    {
        if ((uv_x <= gradient[i].pos) && (uv_x >= gradient[i - 1].pos))
        {
            float mix_factor = (uv_x - gradient[i - 1].pos) /
                               (gradient[i].pos - gradient[i - 1].pos);
            return mix(gradient[i - 1].col, gradient[i].col, mix_factor);
        }
    }
    if ((uv_x <= 1.f) && (uv_x >= gradient[number_of_marks - 1].pos))
    {
        return gradient[number_of_marks - 1].col;
    }
}

#if METHOD == 1

void main()
{
    for (int i = 0; i < benchmark; ++i)
    {
        vec4 color = vec4(0);
        if (gradient_data.length() != 0)
        {
            color = constant(gradient_data, _uv.x);
        }
        out_Color = color;
    }
}

#elif METHOD == 2

void main()
{
    for (int i = 0; i < benchmark; ++i)
    {
        vec4 color = vec4(0);
        if (gradient_data.length() != 0)
        {
            color = linear(gradient_data, _uv.x);
        }
        out_Color = color;
    }
}
#elif METHOD == 3

void main()
{
    for (int i = 0; i < benchmark; ++i)
    {
        vec4 color = vec4(0);
        if (gradient_data.length() != 0)
        {
            color = constant_dicho(gradient_data, _uv.x);
        }
        out_Color = color;
    }
}

#elif METHOD == 4

void main()
{
    for (int i = 0; i < benchmark; ++i)
    {
        vec4 color = vec4(0);
        if (gradient_data.length() != 0)
        {
            color = linear_dicho(gradient_data, _uv.x);
        }
        out_Color = color;
    }
}
#endif
#endif
