#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float r_affect; // default 1 min = 0
uniform float g_affect; // default 1 min = 0
uniform float b_affect; // default 1 min = 0

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 uv     = _uv;
    vec3 color  = vec3(1.);
    vec4 img    = image(uv);
    vec3 affect = vec3(r_affect, g_affect, b_affect);
    color -= affect * img.rgb;
    out_Color = vec4(color, 1.);
}