vec3 image(vec2 uv)
{
    return texture(_image, uv).rgb;
}