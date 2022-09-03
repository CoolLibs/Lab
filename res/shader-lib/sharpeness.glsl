// from https://www.shadertoy.com/view/llXGWf

// Sharpness kernel
// -x   -x    -x
// -x  x*8+1  -x
// -x   -x    -x

vec3 texsample(const int x, const int y, vec2 in_uv)
{
    vec2 uv = in_uv.xy;
    uv      = (uv + vec2(x, y));
    return image(uv);
}

vec3 texfilter(
    vec2 in_uv, float effect_intensity
)
{
    vec3 sum = texsample(-1, -1, in_uv) * -effect_intensity + texsample(-1, 0, in_uv) * -effect_intensity + texsample(-1, 1, in_uv) * -effect_intensity + texsample(0, -1, in_uv) * -effect_intensity + texsample(0, 0, in_uv) * (effect_intensity * 8. + 1.) // sum should always be +1
               + texsample(0, 1, in_uv) * -effect_intensity + texsample(1, -1, in_uv) * -effect_intensity + texsample(1, 0, in_uv) * -effect_intensity + texsample(1, 1, in_uv) * -effect_intensity;

    return sum;
}