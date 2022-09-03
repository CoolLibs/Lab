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

vec2 barrel_disortion(
    vec2 in_uv, float Effect_intensity,
    vec2 Center
)
{
    vec2 uv = in_uv / -1; // invert
    uv      = Center - distort(uv + Center, Effect_intensity + 1.);
    return uv;
}