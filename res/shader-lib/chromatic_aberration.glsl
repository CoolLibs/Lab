// https://www.shadertoy.com/view/4tlyD8

mat3x2 matrix_increments(vec2 uv, float falloff, float blur, float inverseSampleCount)
{
    vec2 direction = normalize(uv);
    vec2 velocity  = direction * blur * pow(length(uv), falloff);

    return mat3x2(
        velocity * 1.0 * inverseSampleCount,
        velocity * 2.0 * inverseSampleCount,
        velocity * 4.0 * inverseSampleCount
    );
}
// TODO(ASG) Remove this file, move everthing to the .frag.
// TODO(ASG) Handle uvs properly
vec3 chromatic_aberration(
    vec3 in_color, float effect_intensity, vec2 uv,
    int sample_count, float falloff, float blur
)
{
    float  inverseSampleCount = 1.0 / sample_count;
    mat3x2 increments         = matrix_increments(uv, falloff, blur, inverseSampleCount);

    vec3   accumulator = vec3(0);
    mat3x2 offsets     = mat3x2(0);
    for (int i = 0; i < sample_count; i++)
    {
        accumulator.g += image(uv + offsets[1]).g;
        accumulator.b += image(uv + offsets[2]).b;
        accumulator.r += image(uv + offsets[0]).r;

        offsets -= increments;
    }

    vec3 out_color = accumulator * inverseSampleCount;
    return mix(in_color, out_color, effect_intensity);
}