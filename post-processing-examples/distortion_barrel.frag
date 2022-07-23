#version 410

// https://www.shadertoy.com/view/ls2XWD

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"

input float center_x;   // default 0.5 range 0 to 1
input float center_y;   // default 0.5 range 0 to 1
input float distortion; // -5 to 5
input float nb_tiles;   // 0 forbbiden

vec2 mid;

vec2 distort(vec2 p, float power)
{
    // Convert to polar coords:
    float theta  = atan(p.y, p.x);
    float radius = length(p);

    // Distort:
    radius = pow(radius, power);

    // Convert back to Cartesian:
    p.x = radius * cos(theta);
    p.y = radius * sin(theta);

    return p;
}

vec4 textureaspectCompensated(sampler2D channel, vec2 pixel)
{
    vec2 uv = vec2(pixel.x * _aspect_ratio, pixel.y);
    return texture(channel, uv);
}

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 pixel = _uv;

    mid = vec2(center_x, center_y);

    vec2 uv = _uv;
    uv /= -1; // normalize and invert

    // Put origo at the center of the viewport.
    uv += mid;

    out_Color = textureaspectCompensated(_image, -distort(uv, distortion) * nb_tiles + mid);
}