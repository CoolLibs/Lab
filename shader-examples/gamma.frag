#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float Gamma;

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

vec3 gamma_modifier(vec3 image)
{
    // return vec3(pow(abs(image.r), Gamma), pow(abs(image.g), Gamma), pow(abs(image.b), Gamma));
    return pow(abs(image), vec3(Gamma));
}

void main()
{
    out_Color = vec4(gamma_modifier(image(_uv).rgb), 1.);
}