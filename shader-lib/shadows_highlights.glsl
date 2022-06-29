// TODO : Must be delete ? / not the best way to implement that, with too mush parameters.

RgbColor accentuate_shadows(RgbColor image, float factor_shadows, float power_shadows, float dir_shadows)
{
    return image + image * -dir_shadows * exp(-factor_shadows * pow(image, RgbColor(power_shadows)));
}

RgbColor accentuate_highlights(RgbColor image, float factor_highlights, float power_highlights, float dir_highlights)
{
    return image + image * dir_highlights * exp(-factor_highlights * pow(image, RgbColor(power_highlights)));
}

RgbColor shadows_highlights(
    RgbColor in_color, float effect_intensity,
    float factor_shadows, float power_shadows, float dir_shadows,
    float factor_highlights, float power_highlights, float dir_highlights
)
{
    RgbColor out_color = accentuate_shadows(accentuate_highlights(in_color, factor_highlights, power_highlights, dir_highlights), factor_shadows, power_shadows, dir_shadows);
    return mix(in_color, out_color, effect_intensity);
}