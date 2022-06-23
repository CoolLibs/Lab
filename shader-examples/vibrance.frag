#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float Vibrance; // default 0.

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

vec3 add_vibrance(vec3 image)
{
    float average   = dot(image, vec3(1 / 3));
    float color_max = max(image.r, max(image.g, image.b));
    float amount    = (color_max - average) * Vibrance * 3. * 5.;
    return image - (color_max - image) * amount;
}

void main()
{
    out_Color = vec4(add_vibrance(image(_uv).rgb), 1.);
}