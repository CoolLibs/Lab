#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float scale; // default 384
uniform float seed;  // default 48

// END DYNAMIC PARAMS

// https://www.shadertoy.com/view/ltB3zD
// Gold Noise ©2015 dcerisano@standard3d.com
// - based on the Golden Ratio
// - uniform normalized distribution
// - fastest static noise generator function (also runs at low precision)
// - use with indicated fractional seeding method.

float PHI = 1.61803398874989484820459; // Φ = Golden Ratio

float gold_noise(in vec2 xy, in float seed)
{
    return fract(tan(distance(xy * PHI, xy) * seed) * xy.x);
}

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 uv = _uv;
    uv.x *= _aspect_ratio;

    // float seed = fract(seed_fract); // fractional base seed

    out_Color = vec4(gold_noise(uv * scale, seed + 0.1),  // r
                     gold_noise(uv * scale, seed + 0.2),  // g
                     gold_noise(uv * scale, seed + 0.3),  // b
                     gold_noise(uv * scale, seed + 0.4)); // α
}