#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform vec3 color1;
uniform vec3 color2;
uniform vec3 color3;

// END DYNAMIC PARAMS

const int size                  = 3;
vec3      table_of_colors[size] = {color1, color2, color3};

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 uv       = _uv;
    vec3 image    = image(_uv).rgb;
    vec2 min_dist = vec2(distance(image, table_of_colors[0]), 0);
    for (int i = 1; i < size; i++) {
        if (distance(image, table_of_colors[i]) < min_dist.x)
            min_dist = vec2(distance(image, table_of_colors[i]), i);
    }
    image     = table_of_colors[int(min_dist.y)];
    out_Color = vec4(image, 1.);
}