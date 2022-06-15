#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float factor_shadows;
uniform float power_shadows;
uniform float dir_shadows;
uniform float factor_highlights;
uniform float power_highlights;
uniform float dir_highlights;

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

vec3 accentuate_shadows(vec3 image)
{
    return image + image * -dir_shadows * exp(-factor_shadows * pow(image, vec3(power_shadows)));
}

vec3 accentuate_highlights(vec3 image)
{
    return image + image * dir_highlights * exp(-factor_highlights * pow(image, vec3(power_highlights)));
}

void main()
{
    out_Color = vec4(accentuate_shadows(accentuate_highlights(image(_uv).xyz)), 1.);
}