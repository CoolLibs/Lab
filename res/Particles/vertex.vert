#version 430

layout(location = 0) in vec2 _position;
layout(location = 1) in vec2 _uv;

out vec2 _varying_uv;
flat out uint _particle_index;

uniform mat3 _camera2D_inverse;
uniform float _inverse_aspect_ratio;

layout(std430, binding = 0) buffer _positions_buffer
{
    float _positions[];
};

layout(std430, binding = 2) buffer _sizes_buffer
{
    float _sizes[];
};

void main()
{
    _varying_uv   = _uv;
    vec2 pos      = _position  * _sizes[gl_InstanceID]+ vec2(_positions[2 * gl_InstanceID] , _positions[2 * gl_InstanceID + 1]);
    vec3 proj_pos = _camera2D_inverse * vec3(pos, 1.);
    proj_pos.xy  *= vec2(_inverse_aspect_ratio, 1.);
    gl_Position   = vec4((proj_pos.xy / proj_pos.z), 0., 1.);

    _particle_index = gl_InstanceID;
}