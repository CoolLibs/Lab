#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float Red_contribution;   // default 0.2126
uniform float Green_contribution; // default 0.7152
uniform float Blue_contribution;  // default 0.0722
uniform float Normalize_checkbox;

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec3 channels_contribution = vec3(Red_contribution, Green_contribution, Blue_contribution);
    if (Normalize_checkbox >= 0.5) {
        float sum_of_channels_contribution = dot(channels_contribution, vec3(1.));
        channels_contribution /= sum_of_channels_contribution;
    }
    float luminosity = dot(image(_uv).rgb, channels_contribution);

    out_Color = vec4(vec3(luminosity), 1.);
}