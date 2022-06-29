// https://www.shadertoy.com/view/Xlc3D2

float rnd(vec2 p)
{
    float f = fract(sin(dot(p, vec2(12.1234, 72.8392)) * 45123.2));
    return f;
}

float rnd(float w)
{
    float f = fract(sin(w) * 1000.);
    return f;
}

float regShape(vec2 p, int N)
{
    float f;

    float a = atan(p.x, p.y) + .2;
    float b = 6.28319 / float(N);
    f       = smoothstep(.5, .51, cos(floor(.5 + a / b) * b - a) * length(p.xy));

    return f;
}
RgbColor circle(vec2 p, float size, float decay, RgbColor color, RgbColor color2, float dist, vec2 mouse)
{
    // l is used for making rings.I get the length and pass it through a sinwave
    // but I also use a pow function. pow function + sin function , from 0 and up, = a pulse, at least
    // if you return the max of that and 0.0.

    float l = length(p + mouse * (dist * 4.)) + size / 2.;

    // l2 is used in the rings as well...somehow...
    float l2 = length(p + mouse * (dist * 4.)) + size / 3.;

    /// these are circles, big, rings, and  tiny respectively
    float c  = max(00.01 - pow(length(p + mouse * dist), size * 1.4), 0.0) * 50.;
    float c1 = max(0.001 - pow(l - 0.3, 1. / 40.) + sin(l * 30.), 0.0) * 3.;
    float c2 = max(0.04 / pow(length(p - mouse * dist / 2. + 0.09) * 1., 1.), 0.0) / 20.;
    float s  = max(00.01 - pow(regShape(p * 5. + mouse * dist * 5. + 0.9, 6), 1.), 0.0) * 5.;

    color      = 0.5 + 0.5 * sin(color);
    color      = cos(RgbColor(0.44, .24, .2) * 8. + dist * 4.) * 0.5 + .5;
    RgbColor f = c * color;
    f += c1 * color;

    f += c2 * color;
    f += s * color;
    return f - 0.01;
}

float sun(vec2 p, vec2 mouse)
{
    float f;

    vec2  sunp = p + mouse;
    float sun  = 1.0 - length(sunp) * 8.;
    return f;
}

RgbColor flare(
    RgbColor in_color, float effect_intensity,
    float x, float y, float Brightness, int Nb_of_circles, int Seed
)
{
    vec2 uv = _uv - 0.5;
    uv.x *= _aspect_ratio;

    vec2 mm = vec2(x, y) - 0.5;
    mm.x *= _aspect_ratio;

    RgbColor circColor  = RgbColor(0.9, 0.2, 0.1);
    RgbColor circColor2 = RgbColor(0.3, 0.1, 0.9);

    // now to make the sky not black
    RgbColor color = in_color;
    RgbColor flare = mix(RgbColor(0.3, 0.2, 0.02) / 0.9, RgbColor(0.2, 0.5, 0.8), uv.y) * 3. - Brightness;

    // this calls the function which adds three circle types every time through the loop based on parameters I
    // got by trying things out. rnd i*2000. and rnd i*20 are just to help randomize things more
    for (float i = 0.; i < Nb_of_circles; i++)
    {
        flare += circle(uv, pow(rnd(i * Seed * 100.) * 1.8, 2.) + 1.41, 0.0, circColor + i, circColor2 + i, rnd(i * Seed) * 3. + 0.2 - .5, mm);
    }
    // get angle and length of the sun (uv - mouse)
    float a = atan(uv.y - mm.y, uv.x - mm.x);
    float l = max(1.0 - length(uv - mm) - 0.84, 0.0);

    // multiply by the exponetial e^x ? of 1.0-length which kind of masks the brightness more so that
    // there is a sharper roll of of the light decay from the sun.
    flare *= exp(1.0 - length(uv - mm)) / 5.;
    color += flare;
    RgbColor out_color = color;
    return mix(in_color, out_color, effect_intensity);
}