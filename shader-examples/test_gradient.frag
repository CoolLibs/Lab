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

#define TEST_IDX 1
const int number_of_marks = TEST_IDX;

#if TEST_IDX == 0

Mark gradient_data[];

#elif TEST_IDX == 1

Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(1, 0, 0, 1))
);

#elif TEST_IDX == 2

Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.15f, vec4(0, 0, .1, 1))

);

#elif TEST_IDX == 3

Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.3f, vec4(0, .1, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.55f, vec4(0.3, 1, 0, 1)),
    Mark(0.75f, vec4(0.5, 1, 0, 1)),
    Mark(0.8f, vec4(1, 1, 0, 1)),
    Mark(0.85f, vec4(1, 0.25, 0.5, 1)),
    Mark(0.95f, vec4(1, 0, 0.8, 1))
);

#elif TEST_IDX == 4

Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.3f, vec4(0, .1, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.55f, vec4(0.3, 1, 0, 1)),
    Mark(0.75f, vec4(0.5, 1, 0, 1)),
    Mark(0.8f, vec4(1, 1, 0, 1)),
    Mark(0.85f, vec4(1, 0.25, 0.5, 1)),
    Mark(0.95f, vec4(1, 0, 0.8, 1))
);

#endif

#if TEST_IDX == 0

void main()
{
    for (int i = 0; i < 2000; ++i)
    {
        vec4 color = vec4(0);
        out_Color  = color;
    }
}

#else

vec4 constant(Mark gradient[number_of_marks])
{
    if ((_uv.x <= gradient_data[0].pos) && (_uv.x >= 0.f))
    {
        return gradient_data[0].col;
    }
    for (int i = 1; i < number_of_marks; i++)
    {
        if ((_uv.x <= gradient_data[i].pos) && (_uv.x >= gradient_data[i - 1].pos))
        {
            return gradient_data[i - 1].col;
        }
    }
    if ((_uv.x <= 1.f) && (_uv.x >= gradient_data[number_of_marks - 1].pos))
    {
        return gradient_data[number_of_marks - 1].col;
    }
}

// TODO(ASG) Use a dichotomic search (and then benchmark to see which method is fastest)

vec4 linear(Mark gradient[number_of_marks])
{
    if ((_uv.x <= gradient_data[0].pos) && (_uv.x >= 0.f))
    {
        return gradient_data[0].col;
    }
    for (int i = 1; i < number_of_marks; i++)
    {
        if ((_uv.x <= gradient_data[i].pos) && (_uv.x >= gradient_data[i - 1].pos))
        {
            float mix_factor = (_uv.x - gradient_data[i - 1].pos) /
                               (gradient_data[i].pos - gradient_data[i - 1].pos);
            return mix(gradient_data[i - 1].col, gradient_data[i].col, mix_factor);
        }
    }
    if ((_uv.x <= 1.f) && (_uv.x >= gradient_data[number_of_marks - 1].pos))
    {
        return gradient_data[number_of_marks - 1].col;
    }
}

void main()
{
    for (int i = 0; i < 2000; ++i)
    {
        vec4 color = vec4(0);
        if (gradient_data.length() != 0)
        {
            color = constant(gradient_data);
        }
        out_Color = color;
    }
}

#endif
