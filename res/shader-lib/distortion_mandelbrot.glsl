// https://www.shadertoy.com/view/MsXXD7

vec2 distortion_mandelbrot(
    vec2 in_uv, float effect_intensity,
    vec2 center, float scale, float subdivision
)
{
    vec2 C = (in_uv - center) * scale;
    // position better to see the fractal
    C = -C.yx * 1.8 + vec2(-.5, 0);

    float n = subdivision;
    vec2  Z = C * fract(n);
    for (int i = 0; i < int(subdivision); i++)
    {
        if (float(i) > n)
            break;
        Z = vec2(Z.x * Z.x - Z.y * Z.y, 2.0 * Z.x * Z.y) + C;
    }

    // image map
    return mix(in_uv, (-Z.yx * .3 + .5 + center.xy) / scale, effect_intensity);
}