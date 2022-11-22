//
// GLSL textureless classic 2D noise "cnoise",
// with an RSL-style periodic variant "pnoise".
// Author:  Stefan Gustavson (stefan.gustavson@liu.se)
// Version: 2011-08-22
//
// Many thanks to Ian McEwan of Ashima Arts for the
// ideas for permutation and gradient selection.
//
// Copyright (c) 2011 Stefan Gustavson. All rights reserved.
// Distributed under the MIT license. See LICENSE file.
// https://github.com/stegu/webgl-noise

vec4 mod289(vec4 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x)
{
    return mod289(((x * 34.0) + 10.0) * x);
}

vec4 taylorInvSqrt(vec4 r)
{
    return 1.79284291400159 - 0.85373472095314 * r;
}

vec2 fade(vec2 t)
{
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

// Classic Perlin noise
float cnoise(vec2 P)
{
    vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
    vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
    Pi      = mod289(Pi); // To avoid truncation effects in permutation
    vec4 ix = Pi.xzxz;
    vec4 iy = Pi.yyww;
    vec4 fx = Pf.xzxz;
    vec4 fy = Pf.yyww;

    vec4 i = permute(permute(ix) + iy);

    vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0;
    vec4 gy = abs(gx) - 0.5;
    vec4 tx = floor(gx + 0.5);
    gx      = gx - tx;

    vec2 g00 = vec2(gx.x, gy.x);
    vec2 g10 = vec2(gx.y, gy.y);
    vec2 g01 = vec2(gx.z, gy.z);
    vec2 g11 = vec2(gx.w, gy.w);

    vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
    g00 *= norm.x;
    g01 *= norm.y;
    g10 *= norm.z;
    g11 *= norm.w;

    float n00 = dot(g00, vec2(fx.x, fy.x));
    float n10 = dot(g10, vec2(fx.y, fy.y));
    float n01 = dot(g01, vec2(fx.z, fy.z));
    float n11 = dot(g11, vec2(fx.w, fy.w));

    vec2  fade_xy = fade(Pf.xy);
    vec2  n_x     = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
    float n_xy    = mix(n_x.x, n_x.y, fade_xy.y);
    return 2.3 * n_xy;
}

// Classic Perlin noise, periodic variant
float pnoise(vec2 P, vec2 rep)
{
    vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
    vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
    Pi      = mod(Pi, rep.xyxy); // To create noise with explicit period
    Pi      = mod289(Pi);        // To avoid truncation effects in permutation
    vec4 ix = Pi.xzxz;
    vec4 iy = Pi.yyww;
    vec4 fx = Pf.xzxz;
    vec4 fy = Pf.yyww;

    vec4 i = permute(permute(ix) + iy);

    vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0;
    vec4 gy = abs(gx) - 0.5;
    vec4 tx = floor(gx + 0.5);
    gx      = gx - tx;

    vec2 g00 = vec2(gx.x, gy.x);
    vec2 g10 = vec2(gx.y, gy.y);
    vec2 g01 = vec2(gx.z, gy.z);
    vec2 g11 = vec2(gx.w, gy.w);

    vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
    g00 *= norm.x;
    g01 *= norm.y;
    g10 *= norm.z;
    g11 *= norm.w;

    float n00 = dot(g00, vec2(fx.x, fy.x));
    float n10 = dot(g10, vec2(fx.y, fy.y));
    float n01 = dot(g01, vec2(fx.z, fy.z));
    float n11 = dot(g11, vec2(fx.w, fy.w));

    vec2  fade_xy = fade(Pf.xy);
    vec2  n_x     = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
    float n_xy    = mix(n_x.x, n_x.y, fade_xy.y);
    return 2.3 * n_xy;
}

float sgn(float x)
{
    return (x > 0) ? 1 : -1;
}

float triangle2(int i, float frequency)
{
    float a = 2;

    if (i > abs(frequency + a))
    {
        return 0;
    }
    else
    {
        float slope_at_i = ((frequency + a - i) * 1) / a;
        return slope_at_i;
    }
}

float triangle(float current_freq, float frequency, float a)
{
    if (abs(current_freq - frequency) > a)
    {
        return 0;
    }
    else
    {
        if (current_freq > frequency)
        {
            return (frequency - current_freq + a * 1) / a;
        }
        else
        {
            return (frequency - current_freq - a * 1) / (-a);
        }
    }
}

float classic_noise(
    vec2  uv,
    float frequency, float fractalness, float scale
)
{
    float noise         = 0;
    float frequency_log = log2(frequency);

    int   start = int(floor(frequency_log - fractalness));
    int   end   = int(ceil(frequency_log + fractalness));
    float sum   = 0;
    for (int i = start; i <= end; i++)
    {
        float current_freq = pow(2., i);
        sum += triangle(i, frequency_log, fractalness);
        noise += (cnoise(uv * current_freq * scale) * 0.5 + 0.5) * triangle(i, frequency_log, fractalness);
    }
    noise /= sum;
    return noise;
}

vec2 classic_noise_uv(
    vec2 uv, float mask,
    float frequency, float fractalness, float scale
)
{
    return mix(uv, vec2(classic_noise(uv, frequency, fractalness, scale)), mask);
}

vec3 classic_noise_color(
    vec2  uv,
    float frequency, float fractalness, float scale
)
{
    return vec3(classic_noise(uv, frequency, fractalness, scale));
}