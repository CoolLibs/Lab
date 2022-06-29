RgbColor gamma_modifier(
    RgbColor in_color, float effect_intensity,
    float gamma
)
{
    RgbColor out_color = pow(min(max(in_color, 0.), 1.), RgbColor(gamma));
    return mix(in_color, out_color, effect_intensity);
}