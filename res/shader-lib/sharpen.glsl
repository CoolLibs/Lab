// from https://www.shadertoy.com/view/llXGWf

// Sharpness kernel
// -x   -x    -x
// -x  x*8+1  -x
// -x   -x    -x

vec3 sampling(float spread, float x, float y, vec2 uv)
{
    return image(uv + spread / 1000.f * vec2(x, y));
}

vec3 sharpen(vec2 in_uv, float effect_intensity, float spread)
{
    vec3 sum = sampling(spread, -1, -1, in_uv) * -effect_intensity
               + sampling(spread, -1, 0, in_uv) * -effect_intensity
               + sampling(spread, -1, 1, in_uv) * -effect_intensity
               + sampling(spread, 0, -1, in_uv) * -effect_intensity
               + sampling(spread, 0, 0, in_uv) * (effect_intensity * 8. + 1.)
               + sampling(spread, 0, 1, in_uv) * -effect_intensity
               + sampling(spread, 1, -1, in_uv) * -effect_intensity
               + sampling(spread, 1, 0, in_uv) * -effect_intensity
               + sampling(spread, 1, 1, in_uv) * -effect_intensity;

    return sum;
}