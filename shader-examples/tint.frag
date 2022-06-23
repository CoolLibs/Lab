#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float Tint; // default 0.0 / default bounds -1.0 -> 1.0

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

vec3 tint_shift(vec3 image)
{
    const vec3 kRGBToYPrime = vec3(0.299, 0.587, 0.114);
    const vec3 kRGBToI      = vec3(0.596, -0.275, -0.321);
    const vec3 kRGBToQ      = vec3(0.212, -0.523, 0.311);

    const vec3 kYIQToR = vec3(1.0, 0.956, 0.621);
    const vec3 kYIQToG = vec3(1.0, -0.272, -0.647);
    const vec3 kYIQToB = vec3(1.0, -1.107, 1.704);

    float YPrime      = dot(image, kRGBToYPrime);
    float I           = dot(image, kRGBToI);
    float Q           = dot(image, kRGBToQ);
    float tint_adjust = atan(Q, I);
    float chroma      = sqrt(I * I + Q * Q);

    tint_adjust += Tint;

    Q = chroma * sin(tint_adjust);
    I = chroma * cos(tint_adjust);

    vec3 yIQ = vec3(YPrime, I, Q);

    return vec3(dot(yIQ, kYIQToR), dot(yIQ, kYIQToG), dot(yIQ, kYIQToB));
}

void main()
{
    out_Color = vec4(tint_shift(image(_uv).rgb), 1.);
}