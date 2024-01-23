#version 430

in vec2      _varying_uv;
flat in uint _particle_index;
out vec4     _out_color;

layout(std430, binding = 5) buffer _colors_buffer
{
    float _colors[];
};

void main()
{
    if (dot(_varying_uv - vec2(0.5), _varying_uv - vec2(0.5)) > 0.25)
        discard;
    _out_color = vec4(
        _colors[_particle_index * 4 + 0],
        _colors[_particle_index * 4 + 1],
        _colors[_particle_index * 4 + 2],
        _colors[_particle_index * 4 + 3]
    );
}