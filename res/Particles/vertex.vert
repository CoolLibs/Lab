#version 430

layout(location = 0) in vec2 _position;
layout(location = 1) in vec2 _uv;

out vec2 _varying_uv;

uniform mat3 _camera2D_inverse;

layout(std430, binding = 0) buffer _positions_buffer
{
    float _positions[];
};

void main()
{
    _varying_uv   = _uv;
    vec2 pos      = _position * 0.01 + vec2(_positions[2 * gl_InstanceID], _positions[2 * gl_InstanceID + 1]);
    vec3 proj_pos = _camera2D_inverse * vec3(pos, 1.);
    gl_Position   = vec4(proj_pos.xy / proj_pos.z, 0., 1.);
}