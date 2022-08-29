// https://www.shadertoy.com/view/ls2XWD

vec2 distort(vec2 pixel, float power)
{
    // Convert to polar coords:
    float theta  = atan(pixel.y, pixel.x);
    float radius = length(pixel);

    // Distort:
    radius = pow(radius, power);

    // Convert back to Cartesian:
    pixel.x = radius * cos(theta);
    pixel.y = radius * sin(theta);

    return pixel;
}

vec4 textureaspectCompensated(sampler2D channel, vec2 pixel)
{
    vec2 uv = vec2(pixel.x * _aspect_ratio, pixel.y);
    return texture(channel, uv);
}
