
layout(location = 0) in vec2 _position;
layout(location = 1) in vec2 _uv;

out vec2      _varying_uv;
flat out uint _particle_index;

uniform mat3  _camera2D_inverse;
uniform mat4  cool_camera_view_projection;
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
    _varying_uv                = _uv;
    float size                 = _sizes[gl_InstanceID];
    mat4  _camera2D_inverse_44 = mat4(
        vec4(_camera2D_inverse[0], 0),
        vec4(_camera2D_inverse[1], 0),
        vec4(0, 0, 0, 0),
        vec4(_camera2D_inverse[2][0], _camera2D_inverse[2][1], 0, 1)
    );
#ifdef IS_3D
    vec3 particle_position = vec3(_positions[3 * gl_InstanceID], _positions[3 * gl_InstanceID + 1], _positions[3 * gl_InstanceID + 2]);
    vec3 camera_right      = vec3(cool_camera_view_projection[0][0], cool_camera_view_projection[1][0], cool_camera_view_projection[2][0]);
    vec3 camera_up         = vec3(cool_camera_view_projection[0][1], cool_camera_view_projection[1][1], cool_camera_view_projection[2][1]);
    vec3 position_up       = camera_up * _position.y * size;
    vec3 position_right    = camera_right * _position.x * size;

    mat4 transform_matrix = _camera2D_inverse_44 * cool_camera_view_projection;
#else
    vec3 particle_position = vec3(_positions[2 * gl_InstanceID], _positions[2 * gl_InstanceID + 1], 0);
    vec3 camera_right      = vec3(1, 0, 0);
    vec3 camera_up         = vec3(0, 1, 0);
    vec3 position_up       = camera_up * _position.y * size;
    vec3 position_right    = camera_right * _position.x * size;
    mat4 transform_matrix  = _camera2D_inverse_44;
#endif
    vec4 proj_pos_3D = transform_matrix * vec4(particle_position + position_right + position_up, 1.);
    gl_Position      = vec4(proj_pos_3D.xyz / proj_pos_3D.w, 1.);
    _particle_index  = gl_InstanceID;
}