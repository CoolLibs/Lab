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
    // if (length(vTexCoords - vec2(0.5)) > 0.5)
    // discard;
    float r    = _colors[_particle_index * 4];
    float g    = _colors[_particle_index * 4 + 1];
    float b    = _colors[_particle_index * 4 + 2];
    float a    = _colors[_particle_index * 4 + 3];
    _out_color = vec4(r, g, b, a);
}