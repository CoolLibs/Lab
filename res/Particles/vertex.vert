
layout(location = 0) in vec2 _position;
layout(location = 1) in vec2 _uv;

out vec2      _varying_uv;
flat out uint _particle_index;

uniform mat4  cool_camera_view;
uniform mat4  transform_matrix;
uniform float _inverse_aspect_ratio;

layout(std430, binding = 0) buffer _positions_buffer
{
    float _positions[];
};

layout(std430, binding = 2) buffer _sizes_buffer
{
    float _sizes[];
};

layout(std430, binding = 3) buffer _lifetimes_buffer
{
    float _lifetimes[];
};

void main()
{
    if (_lifetimes[gl_InstanceID] <= 0)
    {
        gl_Position = vec4(0.);
        return;
    }
    _varying_uv = _uv;
    float size  = _sizes[gl_InstanceID];
#ifdef IS_3D
    vec3 particle_position = vec3(_positions[3 * gl_InstanceID], _positions[3 * gl_InstanceID + 1], _positions[3 * gl_InstanceID + 2]);
    vec3 camera_right      = vec3(cool_camera_view[0][0], cool_camera_view[1][0], cool_camera_view[2][0]);
    vec3 camera_up         = vec3(cool_camera_view[0][1], cool_camera_view[1][1], cool_camera_view[2][1]);
#else
    vec3 particle_position = vec3(_positions[2 * gl_InstanceID], _positions[2 * gl_InstanceID + 1], 0);
    vec3 camera_right      = vec3(1, 0, 0);
    vec3 camera_up         = vec3(0, 1, 0);
#endif
    vec4 proj_pos_3D = transform_matrix
                       * vec4(
                           particle_position
                               + camera_right * _position.x * size
                               + camera_up * _position.y * size,
                           1.
                       );
    proj_pos_3D.x *= _inverse_aspect_ratio;
    gl_Position     = proj_pos_3D;
    _particle_index = gl_InstanceID;
}