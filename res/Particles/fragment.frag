#version 430

in vec2      _varying_uv;
out vec4     _out_color;
uniform int  _size;
uniform bool _first_frame;

layout(std430, binding = 0) buffer _concentration_a_buffer
{
    float _concentration_a[];
};

layout(std430, binding = 1) buffer _concentration_b_buffer
{
    float _concentration_b[];
};

layout(std430, binding = 2) buffer _concentration_a_buffer_2
{
    float _concentration_a2[];
};

layout(std430, binding = 3) buffer _concentration_b_buffer_2
{
    float _concentration_b2[];
};

float read_a(int idx)
{
    return _first_frame ? _concentration_a[idx] : _concentration_a2[idx];
}
float read_b(int idx)
{
    return _first_frame ? _concentration_b[idx] : _concentration_b2[idx];
}

void main()
{
    ivec2 idx  = ivec2(floor(_varying_uv * _size));
    _out_color = vec4(
        vec3(read_b(idx.x + idx.y * _size.x)),
        // idx.x / float(_size), idx.y / float(_size), 0.,
        // _varying_uv, 0.,
        1.
    );
}