#version 410

// https://www.shadertoy.com/view/4tlyD8

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT int     Sample_count;
INPUT float   Falloff;
INPUT float   Blur;
INPUT Point2D Center;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(_uv);
    vec2 uv       = _uv - 0.5;
    uv -= Center;
    uv.x *= _aspect_ratio;

    float  inverseSampleCount = 1.0 / Sample_count;
    vec2   direction          = normalize(uv);
    vec2   velocity           = direction * Blur * pow(length(uv), Falloff) * Effect_intensity;
    mat3x2 increments         = mat3x2(
                velocity * 1.0 * inverseSampleCount,
                velocity * 2.0 * inverseSampleCount,
                velocity * 4.0 * inverseSampleCount
            );

    vec3   accumulator = vec3(0);
    mat3x2 offsets     = mat3x2(0);
    for (int i = 0; i < Sample_count; i++)
    {
        accumulator.g += image(_uv + offsets[1]).g;
        accumulator.b += image(_uv + offsets[2]).b;
        accumulator.r += image(_uv + offsets[0]).r;

        offsets -= increments;
    }

    vec3 color = accumulator * inverseSampleCount;

    out_Color = vec4(color, 1.);
}