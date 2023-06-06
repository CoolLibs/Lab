// Impossible to add "8_" to the beginning of these functions name without create a bug
vec3 bit_color_impl(
    vec3 in_color, float mask,
    float N
)
{
    float n = N / mask;
    return floor(in_color * n) / n;
}

vec3 bit_color(
    vec3 in_color, float mask,
    float N
)
{
    return (mask == 0.)
               ? in_color
               : bit_color_impl(in_color, mask, N);
}
