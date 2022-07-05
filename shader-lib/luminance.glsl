float luminance(vec3 color)
{
    vec3 channels_contribution = vec3(0.2126, 0.7152, 0.0722);
    return dot(color, channels_contribution);
}