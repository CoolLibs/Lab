#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform vec3 Color_coefficient;

uniform float Grey_coefficient;
uniform float One_color_mode;

// END DYNAMIC PARAMS

///-- USER'S DOC --
/// At the grey coefficient given, we replace all pixels at this luminosity by the desired color
/// The rest of the grey levels are mapped automatially according to a power curve.

///-- DEV'S DOC --
/// We use log functions to adapt the color coefficient to the grey coefficient.
/// We just need to choose the desired color and the pow will be automatiquely set to adapt.
/// Initial function was grey^r = color; so, r = log(color)/log(grey).
/// With grey the luminosity factor and r the power's exponent.

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

vec3 color_transformation(vec3 image)
{
    if (One_color_mode > .5)
    {
        image = vec3(dot(image, vec3(0.2126, 0.7152, 0.0722)));
    }
    return pow(image, log(Color_coefficient) / log(Grey_coefficient));
}

void main()
{
    out_Color = vec4(color_transformation(image(_uv).rgb), 1.);
}