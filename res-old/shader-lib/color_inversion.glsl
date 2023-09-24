vec3 color_inversion(
    vec3 in_color, float mask,
    vec3 channels_contribution
)
{
    vec3 out_color = 1. - channels_contribution * in_color;
    return mix(in_color, out_color, mask);
}