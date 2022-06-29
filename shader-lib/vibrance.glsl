RgbColor vibrance(
    RgbColor in_color, float effect_intensity,
    float vibrance
)
{
    float average   = dot(in_color, vec3(1 / 3));
    float color_max = max(in_color.r, max(in_color.g, in_color.b));
    float amount    = (color_max - average) * vibrance * 3. * 5.;

    RgbColor out_color = in_color - (color_max - in_color) * amount;

    return mix(in_color, out_color, effect_intensity);
}