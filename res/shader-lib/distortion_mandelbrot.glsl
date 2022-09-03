// https://www.shadertoy.com/view/MsXXD7

vec2 distortion_mandelbrot(
    vec2 in_uv, float Effect_intensity,
    vec2 Center, float Scale, float Subdivision
)
{
    vec2 C = (in_uv - Center) * Scale;
    // position better to see the fractal
    C = -C.yx * 1.8 + vec2(-.5, 0);

    float n = Subdivision * Effect_intensity;
    vec2  Z = C * fract(n);
    for (int i = 0; i < int(Subdivision); i++)
    {
        if (float(i) > n)
            break;
        Z = vec2(Z.x * Z.x - Z.y * Z.y, 2.0 * Z.x * Z.y) + C;
    }

    // image map
    return mix(in_uv, (-Z.yx * .3 + .5), Effect_intensity);
}