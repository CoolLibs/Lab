vec3 gamma_modifier(
    vec3 in_color, float effect_intensity,
    float gamma
)
{
    vec3 out_color = pow(min(max(in_color, 0.), 1.), vec3(gamma));
    return mix(in_color, out_color, effect_intensity);
}