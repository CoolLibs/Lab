#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform vec3 Color_coefficient;

uniform float Grey_coefficient;

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

float pow_factor(float a, float x)
{
    return pow(x, a);
}

float red_transformation(float red)
{
    return pow_factor(log(Color_coefficient.r) / log(Grey_coefficient), red);
}

float green_transformation(float green)
{
    return pow_factor(log(Color_coefficient.g) / log(Grey_coefficient), green);
}

float blue_transformation(float blue)
{
    return pow_factor(log(Color_coefficient.b) / log(Grey_coefficient), blue);
}

vec3 color_transformation(vec3 image)
{
    float luminosity = image.r * 0.2126 + image.g * 0.7152 + image.b * 0.0722;
    return vec3(red_transformation(luminosity), green_transformation(luminosity), blue_transformation(luminosity));
}

void main()
{
    out_Color = vec4(color_transformation(image(_uv).rgb), 1.);
}