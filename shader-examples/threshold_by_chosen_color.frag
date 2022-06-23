#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform int number_of_colors;

uniform vec3 Color2;

// END DYNAMIC PARAMS
// uniform vec3 Color1;

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    float t      = _time * 0.4;
    t            = smoothstep(0., 3.7, t);
    vec3  Color1 = min(max(vec3(t), 0.), 1.);
    vec2  uv     = _uv;
    vec3  image  = image(_uv).rgb;
    float f      = distance(Color1, image) / ((distance(Color1, image) + distance(Color2, image)));
    f *= (number_of_colors);
    f = floor(f - 0.00001);
    f /= (number_of_colors - 1);
    out_Color = vec4(mix(Color1, Color2, f), 1.);
}