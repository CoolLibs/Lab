#version 410

// https://www.shadertoy.com/view/ltyGRV

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT float spread;
INPUT float scalar;
INPUT float offs;    // 2
INPUT int   SampNum; // 24
// created by florian berger (flockaroo) - 2016
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

#define PI 3.14159265358979

vec4 getCol(vec2 pos)
{
    vec2  uv = pos;
    vec4  c1 = vec4(image(uv), 1.);
    vec4  c2 = vec4(.4); // gray on greenscreen
    float d  = clamp(dot(c1.xyz, vec3(-0.5, 1.0, -0.5)), 0.0, 1.0);
    return mix(c1, c2, 1.8 * d);
}

vec4 getCol2(vec2 pos)
{
    vec2  uv = pos;
    vec4  c1 = vec4(image(uv), 1.);
    vec4  c2 = vec4(1.5); // bright white on greenscreen
    float d  = clamp(dot(c1.xyz, vec3(-0.5, 1.0, -0.5)), 0.0, 1.0);
    return mix(c1, c2, 1.8 * d);
}

vec2 getGrad(vec2 pos, float delta)
{
    // delta /= spread;
    vec2 d = vec2(delta, 0);
    return vec2(
               dot((getCol(pos + d.xy) - getCol(pos - d.xy)).xyz, vec3(.333)),
               dot((getCol(pos + d.yx) - getCol(pos - d.yx)).xyz, vec3(.333))
           ) /
           delta;
}

vec2 getGrad2(vec2 pos, float delta)
{
    // delta /= spread;
    vec2 d = vec2(delta, 0);
    return vec2(
               dot((getCol2(pos + d.xy) - getCol2(pos - d.xy)).xyz, vec3(.333)),
               dot((getCol2(pos + d.yx) - getCol2(pos - d.yx)).xyz, vec3(.333))
           ) /
           delta;
}

vec4 getRand(vec2 pos)
{
    vec2 uv = pos;
    return vec4(image(uv), 1.);
}

float htPattern(vec2 pos)
{
    float p;
    float r = getRand(pos * .4 / .7 * 1.).x;
    p       = clamp((pow(r + .3, 2.) - .45), 0., 1.);
    return p;
}

float getVal(vec2 pos, float level)
{
    return length(getCol(pos).xyz) + 0.0001 * length(pos - 0.5);
    return dot(getCol(pos).xyz, vec3(.333));
}

vec4 getBWDist(vec2 pos)
{
    return vec4(smoothstep(.9, 1.1, getVal(pos, 0.) * .9 + htPattern(pos * .7)));
}

#define N(a) (a.yx * vec2(1, -1))

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 pos  = in_uv;
    vec2 pos2 = pos;
    vec2 pos3 = pos;
    vec2 pos4 = pos;
    vec2 pos0 = pos;

    vec3 col  = vec3(0.);
    vec3 col2 = vec3(0.);

    float cnt  = 0.;
    float cnt2 = 0.;

    for (int i = 0; i < SampNum; i++)
    {
        // gradient for outlines (gray on green screen)
        vec2 gr  = getGrad(pos, offs) + scalar * .0001 * (getRand(pos).xy - .5);
        vec2 gr2 = getGrad(pos2, offs) + scalar * .0001 * (getRand(pos2).xy - .5);

        // gradient for wash effect (white on green screen)
        vec2 gr3 = getGrad2(pos3, offs) + scalar * .0001 * (getRand(pos3).xy - .5);
        vec2 gr4 = getGrad2(pos4, offs) + scalar * .0001 * (getRand(pos4).xy - .5);

        float grl  = clamp(10. * length(gr), 0., 1.);
        float gr2l = clamp(10. * length(gr2), 0., 1.);

        // outlines:
        // stroke perpendicular to gradient
        pos += .8 * normalize(N(gr));
        pos2 -= .8 * normalize(N(gr2));
        float fact = 1. - float(i) / float(SampNum);
        col += fact * mix(vec3(1.2), getBWDist(pos).xyz * 2., grl);
        col += fact * mix(vec3(1.2), getBWDist(pos2).xyz * 2., gr2l);

        // colors + wash effect on gradients:
        // color gets lost from dark areas
        pos3 += .25 * normalize(gr3) + .5 * (getRand(pos0 * .07).xy - .5);
        // to bright areas
        pos4 -= .5 * normalize(gr4) + .5 * (getRand(pos0 * .07).xy - .5);

        float f1 = 3. * fact;
        float f2 = 4. * (.7 - fact);
        col2 += f1 * (getCol2(pos3).xyz + .25 + .4 * getRand(pos3 * 1.).xyz);
        col2 += f2 * (getCol2(pos4).xyz + .25 + .4 * getRand(pos4 * 1.).xyz);

        cnt2 += f1 + f2;
        cnt += fact;
    }
    // normalize
    col /= cnt * 2.5;
    col2 /= cnt2 * 1.65;

    // outline + color
    col = clamp(clamp(col * .9 + .1, 0., 1.) * col2, 0., 1.);

    out_Color = vec4(col, 1.0);
}
