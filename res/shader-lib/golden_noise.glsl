// https://www.shadertoy.com/view/ltB3zD
// Gold Noise ©2015 dcerisano@standard3d.com
// - based on the Golden Ratio
// - uniform normalized distribution
// - fastest static noise generator function (also runs at low precision)
// - use with indicated fractional seeding method.

float golden_noise(in vec2 xy, in float seed)
{
    const float golden_ratio = 1.61803398874989484820459;
    return fract(tan(distance(xy * golden_ratio * 2000., xy) * seed) * xy.x);
}