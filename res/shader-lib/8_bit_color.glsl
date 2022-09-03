// Impossible to add "8_" to the beginning of these functions name without create a bug
vec3 bit_color_impl(
    vec3 in_color, float effect_intensity,
    float N
)
{
    float n = N / effect_intensity;
    return floor(in_color * n) / n;
}

vec3 bit_color(
    vec3 in_color, float effect_intensity,
    float N
)
{
    return (effect_intensity == 0.)
               ? in_color
               : bit_color_impl(in_color, effect_intensity, N);
}
