vec3 gamma_modifier(
    vec3 in_color, float effect_intensity,
    float gamma, bool should_clamp
)
{
    vec3 color     = should_clamp ? clamp(in_color, 0., 1.) : in_color;
    vec3 out_color = pow(color, vec3(gamma));
    return mix(in_color, out_color, effect_intensity);
}