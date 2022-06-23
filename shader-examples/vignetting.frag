#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float Border_Darkness; // Default 0.262
uniform float Center_Radius;   // default 21.

// END DYNAMIC PARAMS

// https://www.shadertoy.com/view/lsKSWR

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 uv = _uv;
    uv *= 1.0 - uv.yx;
    uv.x *= _aspect_ratio;

    float vig = uv.x * uv.y * Center_Radius;

    vig       = min(max(pow(vig, Border_Darkness), 0.), 1.);
    out_Color = vec4(vec3(image(_uv).rgb * vig), 1.);
}