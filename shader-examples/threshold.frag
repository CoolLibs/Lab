#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform int number_of_colors;

uniform vec3 Couleur1;
uniform vec3 Couleur2;

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
    for (int i = 0; i < number_of_colors; i++) {
        if (avg_luminance <= 1. / number_of_colors * (i + 1) && avg_luminance > 1. / number_of_colors * i)
            image = vec3(1. / (number_of_colors - 1) * i);
    }
    out_Color = vec4(image, 1.);
}