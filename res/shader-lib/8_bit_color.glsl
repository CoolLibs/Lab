vec3 8_bit_color(
    vec3 in_color, float effect_intensity,
    float N
)
{
    if (effect_intensity == 0.)
    {
        return in_color;
    }
    else
    {
        float n = N / effect_intensity;
        return floor(in_color * n) / n;
    }
}