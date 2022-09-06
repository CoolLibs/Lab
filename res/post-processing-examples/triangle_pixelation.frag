#version 410

// https://www.shadertoy.com/view/ls2SDG

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT float scale; // 16

INPUT float Effect_intensity;

bool IsLessThan(vec3 a, vec3 b)
{
    return (a.x <= b.x || a.y <= b.y || a.z <= b.z);
}

vec3 SRGBToLinear(in vec3 sRGBCol)
{
    vec3 linearRGBLo = sRGBCol / 12.92;
    vec3 linearRGBHi = pow((sRGBCol + 0.055) / 1.055, vec3(2.4));
    vec3 linearRGB   = IsLessThan(sRGBCol, vec3(0.04045)) ? linearRGBLo : linearRGBHi;
    return linearRGB;
}

vec3 linearToSRGB(in vec3 linearCol)
{
    vec3 sRGBLo = linearCol * 12.92;
    vec3 sRGBHi = (pow(abs(linearCol), vec3(1.0 / 2.4)) * 1.055) - 0.055;
    vec3 sRGB   = IsLessThan(linearCol, vec3(0.0031308)) ? sRGBLo : sRGBHi;
    return sRGB; // pow( linearCol, vec3( 1.0 / 2.2 ) );
}

//

const int   samples  = 2;
const float fSamples = float(samples * samples * 2 * 2);

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 uv       = in_uv;
    vec2 mosaicUV = floor(uv * scale) / scale;
    uv -= mosaicUV;
    uv *= scale;

    vec2 triOffset = vec2(
        step(1.0 - uv.y, uv.x) / (2.0 * scale),
        step(uv.x, uv.y) / (2.0 * scale)
    );

    vec2 sampleUV = mix(in_uv, mosaicUV + triOffset, Effect_intensity);

    vec3 sample2d = vec3(0.0);
    for (int x = -samples; x < samples; x++)
    {
        for (int y = -samples; y < samples; y++)
        {
            vec2 subSampleUV = sampleUV;
            subSampleUV += (vec2(1., 1.0) / vec2(scale * fSamples)) * vec2(float(samples + x), float(samples + y));
            sample2d += SRGBToLinear(image(subSampleUV));
        }
    }
    sample2d /= fSamples;

    vec3 color = linearToSRGB(sample2d);

    out_Color = vec4(color, 1.);
}