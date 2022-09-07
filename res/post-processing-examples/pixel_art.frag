#version 410
// https://www.shadertoy.com/view/ldXSz4
uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT float        Nb_of_pixels_on_y_axis;
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

    vec2 c     = floor(in_uv * Nb_of_pixels_on_y_axis);
    vec2 coord = c / Nb_of_pixels_on_y_axis;

    vec3 src = image(coord);

    // Track the two best colors
    vec3  dst0 = vec3(0), dst1 = vec3(0);
    float best0 = 1e3, best1 = 1e3;

    for (int i = 0; i < Palette_length; i++)
    {
        vec3  tst = Palette(i);
        float err = compare(src, tst);
        if (err < best0)
        {
            best1 = best0;
            dst1  = dst0;
            best0 = err;
            dst0  = tst;
        }
    }

    best0 = sqrt(best0);
    best1 = sqrt(best1);

    vec3 color =
        mod(c.x + c.y, 2.0) >
                (hash(
                     c * 2.0
                 ) *
                 0.75) +
                    (best1 / (best0 + best1))
            ? dst1
            : dst0;
    color = mix(in_color, color, Effect_intensity);

    out_Color = vec4(color, 1.);
}
