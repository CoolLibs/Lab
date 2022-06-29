RgbColor color_discretisation(
    RgbColor in_color, float effect_intensity,
    RgbColor color1, RgbColor color2, RgbColor color3
)
{
    const int size                  = 3;
    RgbColor  table_of_colors[size] = {color1, color2, color3};
    RgbColor  col                   = table_of_colors[0];
    float     min_dist              = distance(in_color, table_of_colors[0]);
    for (int i = 1; i < size; i++)
    {
        float dist = distance(in_color, table_of_colors[i]);
        if (dist < min_dist)
        {
            min_dist = distance(in_color, table_of_colors[i]);
            col      = table_of_colors[i];
        }
    }
    RgbColor out_color = col;
    return mix(in_color, out_color, effect_intensity);
}