#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float Highlights;

// END DYNAMIC PARAMS

const float a = 1.357697966704323E-01;
const float b = 1.006045552016985E+00;
const float c = 4.674339906510876E-01;
const float d = 8.029414702292208E-01;
const float e = 1.127806558508491E-01;

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

vec4 highlight_modifier(vec4 image)
{
    float maxx    = max(image.r, max(image.g, image.b));
    float minx    = min(image.r, min(image.g, image.b));
    float lum     = (maxx + minx) / 2.0;
    float x1      = abs(Highlights);
    float x2      = lum;
    float lum_new = lum < 0.5
                        ? lum
                        : lum +
                              a * sign(Highlights) * exp(-0.5 * (((x1 - b) / c) * ((x1 - b) / c) + ((x2 - d) / e) * ((x2 - d) / e)));
    // gl_FragColor = image * lum_new / lum;
    return vec4(image * lum_new / lum);
}

void main()
{
    out_Color = highlight_modifier(image(_uv));
}