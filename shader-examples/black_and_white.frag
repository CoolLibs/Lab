#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float Red_coeficient;     // default 0.2126
uniform float Green_coeficient;   // default 0.7152
uniform float Blue_coeficient;    // default 0.0722
uniform float Normalize_checkbox; // default 0.0722

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    float total_colors_coeficients   = Red_coeficient + Green_coeficient + Blue_coeficient;
    vec3  black_and_white_coeficient = vec3(Red_coeficient, Green_coeficient, Blue_coeficient);
    if (Normalize_checkbox >= 0.5) {
        black_and_white_coeficient /= total_colors_coeficients;
    }
    out_Color = vec4(
        vec3(dot(image(_uv).rgb, black_and_white_coeficient)),
        1.);
}