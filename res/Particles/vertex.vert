#version 430

#include "_COOL_RES_/shaders/camera.glsl"

// layout(location = 0) in vec2 _position;
layout(location = 0) in vec3 _position;
layout(location = 1) in vec2 _uv;

out vec2 _varying_uv;

uniform mat3 _camera2D_inverse;

layout(std430, binding = 0) buffer _positions_buffer
{
    float _positions[];
};

void main()
{
    _varying_uv = _uv;

    float size              = .01;
    vec3  particle_position = vec3(_positions[3 * gl_InstanceID], _positions[3 * gl_InstanceID + 1], _positions[3 * gl_InstanceID + 2]);

    vec3 camera_right = vec3(cool_camera_view_projection[0][0], cool_camera_view_projection[1][0], cool_camera_view_projection[2][0]);
    vec3 camera_up    = vec3(cool_camera_view_projection[0][1], cool_camera_view_projection[1][1], cool_camera_view_projection[2][1]);

    mat4 _camera2D_inverse_44 = mat4(
        vec4(_camera2D_inverse[0], 0),
        vec4(_camera2D_inverse[1], 0),
        vec4(0, 0, 1, 0),
        vec4(_camera2D_inverse[2][0], _camera2D_inverse[2][1], 0, 1)
    );

    vec4 proj_pos_3D = _camera2D_inverse_44 * cool_camera_view_projection * vec4(particle_position + camera_right * _position.x * size + camera_up * _position.y * size, 1.);
    gl_Position      = vec4(proj_pos_3D.xyz / proj_pos_3D.w, 1.);
}