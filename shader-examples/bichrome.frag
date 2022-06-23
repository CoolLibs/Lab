#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform vec3 Color1;
uniform vec3 Color2;

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2  uv                    = _uv;
    vec3  image                 = image(_uv).rgb;
    vec3  channels_contribution = vec3(0.2126, 0.7152, 0.0722);
    float avg_luminance         = dot(image, channels_contribution);
    out_Color                   = vec4(mix(Color1, Color2, avg_luminance), 1.);
}