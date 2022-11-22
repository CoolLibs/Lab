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

vec2 distortion_bulging(
    vec2 in_uv, float mask,
    float strength, vec2 center
)
{
    vec2 uv = in_uv / -1; // invert
    uv      = center - distort(uv + center, strength * mask + 1.);
    return uv;
}