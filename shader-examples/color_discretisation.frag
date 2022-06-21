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
    vec2 uv    = _uv;
    vec3 image = image(_uv).rgb;

    vec3  col      = table_of_colors[0];
    float min_dist = distance(image, table_of_colors[0]);
    for (int i = 1; i < size; i++) {
        float dist = distance(image, table_of_colors[i]);
        if (dist < min_dist) {
            min_dist = distance(image, table_of_colors[i]);
            col      = table_of_colors[i];
        }
    }
    out_Color = vec4(col, 1.);
}