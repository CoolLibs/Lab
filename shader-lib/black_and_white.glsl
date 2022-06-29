RgbColor black_and_white(
    RgbColor in_color, float effect_intensity,
    RgbColor channels_contribution, bool normalize_checkbox
)
{
    if (normalize_checkbox)
    {
        float sum_of_channels_contribution = dot(channels_contribution, vec3(1.));
        channels_contribution /= sum_of_channels_contribution;
    }
    float luminosity = dot(in_color, channels_contribution);

    RgbColor out_color = RgbColor(luminosity);
    return mix(in_color, out_color, effect_intensity);
}