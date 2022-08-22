#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

#define TEST_IDX 0
#define METHOD   1 // 1 : constant, 2 : linear, 3 : constant dicho, 4 : linear dicho

const int number_of_marks = TEST_IDX; // folowwing functions need to know gradient size

const int benchmark = 3000;

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

// #include "_ROOT_FOLDER_/shader-examples/gradient/constant_gradient.glsl"
// #include "_ROOT_FOLDER_/shader-examples/gradient/linear_gradient.glsl"
// #include "_ROOT_FOLDER_/shader-examples/gradient/dicho_gradient.glsl"

// #include "_ROOT_FOLDER_/shader-examples/gradient/generated/gradient_test_generated.inl"

// TODO(ASG) benchmark to see which method is fastest

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
