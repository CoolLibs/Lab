// https://www.shadertoy.com/view/4tlyD8

mat3x2 mat_increments(vec2 uv, float falloff, float blur, float inverseSampleCount)
{
    vec2 direction = normalize(uv);
    vec2 velocity  = direction * blur * pow(length(uv), falloff);

    return mat3x2(
        velocity * 1.0 * inverseSampleCount,
        velocity * 2.0 * inverseSampleCount,
        velocity * 4.0 * inverseSampleCount
    );
}

vec3 chromatic_aberration(
    vec3 in_color, float effect_intensity,
    vec2 in_uv, float aspect_ratio,
    vec2 center, int sampleCount, float falloff, float blur
)
{
    vec2 uv = in_uv - center;
    uv.x *= aspect_ratio;

    float  inverseSampleCount = 1.0 / sampleCount;
    mat3x2 increments         = mat_increments(uv, falloff, blur, inverseSampleCount);

    vec3   accumulator = vec3(0);
    mat3x2 offsets     = mat3x2(0);
    for (int i = 0; i < sampleCount; i++)
    {
        accumulator.g += image(in_uv + offsets[1]).g;
        accumulator.b += image(in_uv + offsets[2]).b;
        accumulator.r += image(in_uv + offsets[0]).r;

        offsets -= increments;
    }

    vec3 out_color = accumulator * inverseSampleCount;
    return mix(in_color, out_color, effect_intensity);
}