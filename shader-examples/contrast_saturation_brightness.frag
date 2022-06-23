#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float Contrast;   // default 1.0
uniform float Brightness; // default 1.0
uniform float Saturation; // default 1.0

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

// vec3 brightness_contrast(vec3 image)
//
//     return (image - 0.5) * Contrast + 0.5 + Brightness;
// }

vec3 contrast_saturation_brightness(vec3 color)
{
    const vec3 luminosity_coefficient = vec3(0.2125, 0.7154, 0.0721);
    const vec3 average_luminosity     = vec3(0.5);
    vec3       brightness_color       = color * Brightness;
    vec3       intensity              = vec3(dot(brightness_color, luminosity_coefficient));
    vec3       saturation_color       = mix(intensity, brightness_color, Saturation);
    vec3       contrast_color         = mix(average_luminosity, saturation_color, Contrast);

    return contrast_color;
}

void main()
{
    // out_Color = vec4(brightness_contrast(image(_uv).rgb), 1.);
    out_Color = vec4(contrast_saturation_brightness(image(_uv).rgb), 1.);
}