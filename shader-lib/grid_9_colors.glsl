vec2 grid_9_colors(vec2 in_uv, float effect_intensity)
{
    vec2 out_uv = 3 * in_uv;

    return mix(in_uv, out_uv, effect_intensity);
}