vec3 threshold_by_chosen_color(
    vec3 in_color, float effect_intensity,
    int number_of_colors, vec3 color1, vec3 color2
)
{
    float f = distance(color1, in_color) / ((distance(color1, in_color) + distance(color2, in_color)));
    f *= (number_of_colors);
    f = floor(f - 0.00001);
    f /= (number_of_colors - 1);

    vec3 out_color = mix(color1, color2, f);

    return mix(in_color, out_color, effect_intensity);
}