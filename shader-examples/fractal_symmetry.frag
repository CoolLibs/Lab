#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS
uniform float size;          // 0 forbidden 0.001 to 0.5
uniform float angle_ratio;   // 0 to 360
uniform float right_or_left; // bool
uniform float scale;
uniform float fractal;

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

vec2 u(float angle)
{
    return vec2(sin(angle), cos(angle));
}
void main()
{
    vec2 uv;
    uv = _uv - .5;
    uv.x *= _aspect_ratio;
    uv /= size;
    vec3 color = vec3(0);

    uv.x                = abs(uv.x);
    float fractal_angle = (5. / 6.) * PI;
    uv.y += tan(fractal_angle) * .5;

    vec2  u_line = u(fractal_angle);
    float d      = dot(uv - vec2(.5, 0), u_line);
    uv -= u_line * symmetry_side(right_or_left, d) * 2;

    u_line = u(angle_ratio * (2. / 3.) * PI);
    uv.x += .5;

    for (int i = 0; i < fractal; i++) {
        uv *= scale;
        uv.x -= 1.5;

        uv.x = abs(uv.x);
        uv.x -= .5;
        uv -= u_line * min(0., dot(uv, u_line)) * 2;
    }

    d = length(uv - vec2(clamp(uv.x, -1, 1), 0.));
    uv /= (pow(scale, fractal));

    color += image(uv * size).xyz;
    out_Color = vec4(color, 1.);
}