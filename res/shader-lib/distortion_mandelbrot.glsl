// https://www.shadertoy.com/view/MsXXD7

vec2 distortion_mandelbrot(
    vec2 in_uv, float effect_intensity,
    vec2 center, float scale, float nb_iterations
)
{
    vec2 C = (in_uv - center) * scale;
    // position better to see the fractal
    C = -C.yx * 1.8 + vec2(-.5, 0);

    vec2 Z = C * fract(nb_iterations);
    for (int i = 0; i < int(nb_iterations); i++)
    {
        if (float(i) > nb_iterations)
            break;
        Z = vec2(Z.x * Z.x - Z.y * Z.y, 2.0 * Z.x * Z.y) + C;
    }

    // image map
    vec2 uv = (-Z.yx * .3 + .5 + center) / scale;
    return mix(in_uv, uv, effect_intensity);
}