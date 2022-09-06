#version 410
// https://www.shadertoy.com/view/ldXSz4
uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT float        Pixel_size;
INPUT ColorPalette Palette;

INPUT float Effect_intensity;

// Maps into DawnBringer's 4-bit (16 color) palette http://www.pixeljoint.com/forum/forum_posts.asp?TID=12795
// Also see the amazing ASCII shadertoy: https://www.shadertoy.com/view/lssGDj

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

    vec2 c     = floor(in_uv * Pixel_size);
    vec2 coord = c / Pixel_size;

    vec3 src = image(coord);

    // Track the two best colors
    vec3  dst0 = vec3(0), dst1 = vec3(0);
    float best0 = 1e3, best1 = 1e3;
#define TRY(color)                     \
    {                                  \
        vec3  tst = color;             \
        float err = compare(src, tst); \
        if (err < best0)               \
        {                              \
            best1 = best0;             \
            dst1  = dst0;              \
            best0 = err;               \
            dst0  = tst;               \
        }                              \
    }

    for (int i = 0; i < Palette_length; i++)
    {
        TRY(Palette(i) * Effect_intensity);
    }
    // TRY(vec3(0.078431, 0.047059, 0.109804));
    // TRY(vec3(0.266667, 0.141176, 0.203922));
    // TRY(vec3(0.188235, 0.203922, 0.427451));
    // TRY(vec3(0.305882, 0.290196, 0.305882));
    // TRY(vec3(0.521569, 0.298039, 0.188235));
    // TRY(vec3(0.203922, 0.396078, 0.141176));
    // TRY(vec3(0.815686, 0.274510, 0.282353));
    // TRY(vec3(0.458824, 0.443137, 0.380392));
    // TRY(vec3(0.349020, 0.490196, 0.807843));
    // TRY(vec3(0.823529, 0.490196, 0.172549));
    // TRY(vec3(0.521569, 0.584314, 0.631373));
    // TRY(vec3(0.427451, 0.666667, 0.172549));
    // TRY(vec3(0.823529, 0.666667, 0.600000));
    // TRY(vec3(0.427451, 0.760784, 0.792157));
    // TRY(vec3(0.854902, 0.831373, 0.368627));
    // TRY(vec3(0.870588, 0.933333, 0.839216));
#undef TRY

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
