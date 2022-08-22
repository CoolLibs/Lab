// https://www.shadertoy.com/view/4tlyD8

vec3 chromatic_aberration(
    vec3 in_color, float effect_intensity,
    float center_x, float center_y, int sampleCount, float falloff, float blur
)
{
    vec2 center = vec2(center_x, center_y);
    vec2 uv     = _uv - center;
    uv.x *= _aspect_ratio;

    vec2  direction          = normalize(uv);
    vec2  velocity           = direction * blur * pow(length(uv), falloff);
    float inverseSampleCount = 1.0 / sampleCount;

    mat3x2 increments = mat3x2(
        velocity * 1.0 * inverseSampleCount,
        velocity * 2.0 * inverseSampleCount,
        velocity * 4.0 * inverseSampleCount
    );

    vec3   accumulator = vec3(0);
    mat3x2 offsets     = mat3x2(0);

    for (int i = 0; i < sampleCount; i++)
    {
        accumulator.g += image(_uv + offsets[1]).g;
        accumulator.b += image(_uv + offsets[2]).b;
        accumulator.r += image(_uv + offsets[0]).r;

        offsets -= increments;
    }
    vec3 out_color = accumulator / float(sampleCount);
    return mix(in_color, out_color, effect_intensity);
}