#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS
uniform float center_x; // 0 to 1 default 0.5
uniform float center_y;
uniform float angle_ratio;   // 0 to 360
uniform float right_or_left; // bool

// END DYNAMIC PARAMS

#define PI 3.1415926538

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

float symmetry_side(float right, float ortho)
{
    return (right > 0.5) ? min(0, ortho) : max(0., ortho);
}
void main()
{
    vec2 center = vec2(center_x, center_y);
    vec2 uv     = _uv - center;

    vec3 color = vec3(0);

    float angle  = angle_ratio * PI;
    vec2  u_line = vec2(sin(angle), -cos(angle));
    uv           = uv - u_line * symmetry_side(right_or_left, dot(uv, u_line)) * 2;

    color += image(uv + center).xyz;
    out_Color = vec4(color, 1.);
}
