#version 410

// https://www.shadertoy.com/view/ls2SDG

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT float Scale; // 16

INPUT float Mask;

const int   samples  = 2;
const float fSamples = float(samples * samples * 2 * 2);

void main()
{
    vec2 in_uv = normalized_uv();

    if (Mask == 0.)
    {
        out_Color = vec4(image(in_uv), 1.);
        return;
    }

    float scale = Scale / Mask;

    vec2 uv       = in_uv;
    vec2 mosaicUV = floor(uv * scale) / scale;
    uv -= mosaicUV;
    uv *= scale;

    vec2 triOffset = vec2(
        step(1.0 - uv.y, uv.x) / (2.0 * scale),
        step(uv.x, uv.y) / (2.0 * scale)
    );

    vec2 sampleUV = mosaicUV + triOffset;

    vec3 sample2d = vec3(0.0);
    for (int x = -samples; x < samples; x++)
    {
        for (int y = -samples; y < samples; y++)
        {
            vec2 subSampleUV = sampleUV;
            subSampleUV += (vec2(1., 1.0) / vec2(scale * fSamples)) * vec2(float(samples + x), float(samples + y));
            sample2d += image(subSampleUV);
        }
    }
    sample2d /= fSamples;

    vec3 color = sample2d;

    out_Color = vec4(color, 1.);
}