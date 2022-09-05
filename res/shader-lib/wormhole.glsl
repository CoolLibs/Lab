// https://www.shadertoy.com/view/tdXfW7

vec3 wormhole(
    vec2 in_uv, float effect_intensity,
    float aspect_ratio,
    vec2 center, float atractivity, float symmetry, float intensity, float wormhole_smooth, float wormhole_size, vec3 wormhole_color
)
{
    vec2 p = in_uv - center;
    p.x *= aspect_ratio;

    vec2  tuv   = vec2(atan(p.x, p.y) / PI * symmetry, atractivity / length(p)) + .5;
    float l     = sqrt(p.x * p.x + p.y * p.y);
    vec3  color = vec3(0.);
    if (l > 2.)
    {
        l = wormhole_size;
    }
    else
    {
        l = l * wormhole_smooth - wormhole_size;
        l = -(l * l) + wormhole_size;
    }
    tuv = abs(1. - 2. * tuv);

    color += wormhole_color * effect_intensity;
    color += image(
                 mix(in_uv, tuv, effect_intensity)
             ) *
             mix(1., l * intensity, effect_intensity);

    return color;
}