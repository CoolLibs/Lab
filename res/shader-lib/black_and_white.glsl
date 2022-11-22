vec3 black_and_white(
    vec3 in_color, float mask,
    vec3 channels_contribution, bool Normalize
)
{
    if (Normalize)
    {
        float sum_of_channels_contribution = dot(channels_contribution, vec3(1.));
        channels_contribution /= sum_of_channels_contribution;
    }
    float luminosity = dot(in_color, channels_contribution);

    vec3 out_color = vec3(luminosity);
    return mix(in_color, out_color, mask);
}