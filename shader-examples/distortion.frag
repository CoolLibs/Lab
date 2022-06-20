#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;
uniform sampler2D _texture;

// BEGIN DYNAMIC PARAMS

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec3 color = image(_uv).rgb;
    vec2 uv    = _uv;
    uv.x *= _aspect_ratio;
    vec2 distorted_uv = uv + texture(_texture, _uv).r * .05;
    vec4 screen_color = texture(_image, distorted_uv);
    out_Color         = screen_color;
}