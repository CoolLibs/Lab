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
vec3 circle(vec2 p, float size, float decay, vec3 color, vec3 color2, float dist, vec2 mouse)
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

    color  = 0.5 + 0.5 * sin(color);
    color  = cos(vec3(0.44, .24, .2) * 8. + dist * 4.) * 0.5 + .5;
    vec3 f = c * color;
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

vec3 flare(
    vec3 in_color, float mask, vec2 in_uv,
    vec2 center, vec2 direction, float scale, float brightness, int number_of_circles, int seed
)
{
    vec2 uv = (in_uv * scale - center);
    vec2 mm = direction;

    vec3 circColor  = vec3(0.9, 0.2, 0.1);
    vec3 circColor2 = vec3(0.3, 0.1, 0.9);

    // now to make the sky not black

    vec3 flare = mix(vec3(0.3, 0.2, 0.02) / 0.9, vec3(0.2, 0.5, 0.8), uv.y) * 3. + brightness - 1.;

    // this calls the function which adds three circle types every time through the loop based on parameters I
    // got by trying things out. rnd i*2000. and rnd i*20 are just to help randomize things more
    for (float i = 0.; i < number_of_circles; i++)
    {
        flare += circle(uv, pow(rnd(i * seed * 100.) * 1.8, 2.) + 1.41, 0.0, circColor + i, circColor2 + i, rnd(i * seed) * 3. + 0.2 - .5, mm);
    }
    // get angle and length of the sun (uv - mouse)
    float a = atan(uv.y - mm.y, uv.x - mm.x);
    float l = max(1.0 - length(uv - mm) - 0.84, 0.0);

    // multiply by the exponetial e^x ? of 1.0-length which kind of masks the brightness more so that
    // there is a sharper roll of of the light decay from the sun.
    flare *= exp(1.0 - length(uv - mm)) / 5.;

    return mix(in_color, in_color + flare, mask);
}