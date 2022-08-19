vec3 color_discretisation(
    vec3 in_color, float effect_intensity,
    vec3 color1, vec3 color2, vec3 color3
)
{
    const int size                  = 3;
    vec3      table_of_colors[size] = {color1, color2, color3};
    vec3      col                   = table_of_colors[0];
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
    vec3 out_color = col;
    return mix(in_color, out_color, effect_intensity);
}