// https://www.shadertoy.com/view/tdXfW7

vec3 wormhole(
    vec2 in_uv, float Effect_intensity,
    float aspect_ratio,
    vec2 Center, float Atractivity, float Symmetry, float Intensity, float Wormhole_smooth, float Wormhole_size, RgbColor Wormhole_color
)
{
    vec2 p = in_uv - Center;
    p.x *= aspect_ratio;

    vec2  tuv   = vec2(atan(p.x, p.y) / PI * Symmetry, Atractivity / length(p)) + .5;
    float l     = sqrt(p.x * p.x + p.y * p.y);
    vec3  color = vec3(0.);
    if (l > 2.)
    {
        l = Wormhole_size;
    }
    else
    {
        l = l * Wormhole_smooth - Wormhole_size;
        l = -(l * l) + Wormhole_size;
    }
    tuv = abs(1. - 2. * tuv);

    color += Wormhole_color * Effect_intensity;
    color += image(
                 mix(in_uv, tuv, Effect_intensity)
             ) *
             mix(1., l * Intensity, Effect_intensity);

    return color;
}