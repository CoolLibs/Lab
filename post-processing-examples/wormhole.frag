#version 430

// https://www.shadertoy.com/view/tdXfW7

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float atractivity; // default 0.008
uniform float symmetry;    // min = 0 max = 0.5
uniform float intensity;
uniform float wormhole_smooth; // fobidden 0
uniform float wormhole_size;   // fobidden 0

uniform vec3 wormhole_color;

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 p = _uv - 0.5;
    p.x *= _aspect_ratio;

    vec2  tuv   = vec2(atan(p.x, p.y) / PI * symmetry, atractivity / length(p)) + .5;
    float l     = sqrt(p.x * p.x + p.y * p.y);
    vec3  color = vec3(0.);
    if (l > 2.)
    {
        l = wormhole_size;
    }
    else
    {
        l = l * wormhole_smooth - wormhole_size;
        l = -(l * l) + wormhole_size;
    }
    tuv = abs(1. - 2. * tuv);
    color += wormhole_color;
    color += image(tuv).rgb * l * intensity;
    out_Color = vec4(color, 1.);
}
