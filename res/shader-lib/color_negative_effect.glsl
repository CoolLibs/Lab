vec3 color_negative_effet(
    vec3 in_color, float Effect_intensity,
    vec3 channels_contribution
)
{
    return mix(in_color, vec3(1.) - channels_contribution * in_color, Effect_intensity);
}