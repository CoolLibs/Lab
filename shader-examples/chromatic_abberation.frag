#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform int sampleCount;

uniform float falloff;

// END DYNAMIC PARAMS
uniform float blur;

vec3 image(vec2 uv)
{
    return texture2D(_image, uv).rgb;
}

// https://www.shadertoy.com/view/4tlyD8

void main()
{
    float time = _time * 0.5;
    float blur = -cos(time * 3.) * 0.5 + 0.5;
    blur       = pow(blur, 5.);
    blur += pow(-cos(time * 3. + 1.7) * 0.5 + 0.5, 8.);
    blur                     = mix(0., 0.5, blur);
    vec2  direction          = normalize(_uv - 0.5);
    vec2  velocity           = direction * blur * pow(length(_uv - 0.5), falloff);
    float inverseSampleCount = 1.0 / float(sampleCount);

    mat3x2 increments = mat3x2(velocity * 1.0 * inverseSampleCount,
                               velocity * 2.0 * inverseSampleCount,
                               velocity * 4.0 * inverseSampleCount);

    vec3   accumulator = vec3(0);
    mat3x2 offsets     = mat3x2(0);

    for (int i = 0; i < sampleCount; i++) {
        accumulator.r += image(_uv + offsets[0]).r;
        accumulator.g += image(_uv + offsets[1]).g;
        accumulator.b += image(_uv + offsets[2]).b;

        offsets -= increments;
    }

    out_Color = vec4(accumulator / float(sampleCount), 1.);
}