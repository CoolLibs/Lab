vec3 highlight_modifier(
    vec3 in_color, float effect_intensity,
    float highlights
)
{
    const float a         = 1.357697966704323E-01;
    const float b         = 1.006045552016985E+00;
    const float c         = 4.674339906510876E-01;
    const float d         = 8.029414702292208E-01;
    const float e         = 1.127806558508491E-01;
    float       maxx      = max(in_color.r, max(in_color.g, in_color.b));
    float       minx      = min(in_color.r, min(in_color.g, in_color.b));
    float       lum       = (maxx + minx) / 2.0;
    float       x1        = abs(highlights);
    float       x2        = lum;
    float       lum_new   = lum < 0.5
                                ? lum
                                : lum +
                              a * sign(highlights) * exp(-0.5 * (((x1 - b) / c) * ((x1 - b) / c) + ((x2 - d) / e) * ((x2 - d) / e)));
    vec3        out_color = in_color * lum_new / lum;
    return mix(in_color, out_color, effect_intensity);
}