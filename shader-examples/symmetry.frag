#version 430

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS
uniform float center_x;       // 0 to 1 default 0.5
uniform float center_y;       // 0 to 1 default 0.5
uniform float angle_in_turns; // 0 to 1 (1 == 1 turn)
uniform float right_or_left;  // bool

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

float symmetry_side(bool right, float ortho)
{
    return right ? min(0, ortho) : max(0., ortho);
}

void main()
{
    vec2  center = vec2(center_x, center_y);
    float angle  = angle_in_turns * TAU;
    vec2  u_line = vec2(sin(angle), -cos(angle));

    vec2 uv = _uv - center;
    uv      = center + uv - u_line * symmetry_side(right_or_left > 0.5, dot(uv, u_line)) * 2.;

    vec3 color = image(uv).xyz;
    out_Color  = vec4(color, 1.);
}
