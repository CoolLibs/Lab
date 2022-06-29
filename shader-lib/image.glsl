RgbColor image(vec2 uv)
{
    return texture2D(_image, uv).rgb;
}