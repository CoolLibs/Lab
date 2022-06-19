#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform int sampleCount; // 20

uniform float falloff; // 1.082
uniform float blur;    // 0.142

uniform float center_x; // 0.142
uniform float center_y; // 0.142

// END DYNAMIC PARAMS

vec3 image(vec2 uv)
{
    return texture2D(_image, uv).rgb;
}

// https://www.shadertoy.com/view/4tlyD8

void main()
{
    vec2 center = vec2(center_x, center_y);
    vec2 uv     = _uv - center;
    uv.x *= _aspect_ratio;

    vec2  direction          = normalize(uv);
    vec2  velocity           = direction * blur * pow(length(uv), falloff);
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