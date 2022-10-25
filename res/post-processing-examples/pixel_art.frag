#version 410
// https://www.shadertoy.com/view/ldXSz4
uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"

INPUT float        Scale;
INPUT ColorPalette Palette;

INPUT float Effect_intensity;

float hash(vec2 p)
{
    return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x))));
}

float compare(vec3 a, vec3 b)
{
    // Increase saturation
    a = max(vec3(0.0), a - min(a.r, min(a.g, a.b)) * 0.25);
    b = max(vec3(0.0), b - min(b.r, min(b.g, b.b)) * 0.25);
    a *= a * a;
    b *= b * b;
    vec3 diff = (a - b);
    return dot(diff, diff);
}

void main()
{
    vec2 in_uv    = normalized_uv();
    vec3 in_color = image(in_uv);

    vec2 c     = floor(in_uv * Scale);
    vec2 coord = c / Scale;

    vec3 src = image(coord);

    // Track the two best colors
    float luminance = clamp(cool__luminance(in_color), 0., 0.99999);

    int idx0 = int(floor(luminance * Palette_length));
    int idx1;
    if (fract(luminance * Palette_length) < 0.5)
    {
        if (idx0 != 0)
            idx1 = idx0 - 1;
        else
            idx1 = idx0 + 1;
    }
    else
    {
        if (idx0 != Palette_length - 1)
            idx1 = idx0 + 1;
        else
            idx1 = idx0 - 1;
    }

    float dist0 = sqrt(distance(luminance, (idx0 + 0.5) / Palette_length));
    float dist1 = sqrt(distance(luminance, (idx1 + 0.5) / Palette_length));

    vec3 color =
        mod(c.x + c.y, 2.0) > (hash(
                                   c * 2.0
                               )
                               * 0.75)
                                  + (dist1 / (dist0 + dist1))
            ? Palette(idx1)
            : Palette(idx0);
    color = mix(in_color, color, Effect_intensity);

    out_Color = vec4(color, 1.);
}
