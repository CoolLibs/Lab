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
    vec2 uv    = _uv;
    vec3 image = image(_uv).rgb;
    for (int i = 0; i < number_of_colors; i++) {
        if (distance(Couleur1, image) <= (distance(Couleur1, image) + distance(Couleur2, image)) / number_of_colors * (i + 1) && distance(Couleur1, image) > (distance(Couleur1, image) + distance(Couleur2, image)) / number_of_colors * i)
            image = mix(Couleur1, Couleur2, 1. / (number_of_colors - 1) * i);
    }
    // image     = mix(Couleur1, Couleur2, distance(Couleur1, image) / (distance(Couleur1, image) + distance(Couleur2, image)));
    out_Color = vec4(image, 1.);
}