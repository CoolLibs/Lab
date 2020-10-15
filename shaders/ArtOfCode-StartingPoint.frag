// "RayMarching starting point" 
// by Martijn Steinrucken aka The Art of Code/BigWings - 2020
// The MIT License
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// Email: countfrolic@gmail.com
// Twitter: @The_ArtOfCode
// YouTube: youtube.com/TheArtOfCodeIsCool
// Facebook: https://www.facebook.com/groups/theartofcode/
//
// You can use this shader as a template for ray marching shaders

#version 430

varying vec2 vUV;
uniform float uAspectRatio;
uniform vec3 uCamX;
uniform vec3 uCamY;
uniform vec3 uCamZ;
uniform vec3 uCamPos;
uniform float uFocalLength;

uniform float uTime;

#define TAU 6.28

#define MAX_STEPS 100
#define MAX_DIST 10000.
#define SURF_DIST .001

#define S smoothstep
#define T iTime

mat2 Rot(float a) {
    float s=sin(a), c=cos(a);
    return mat2(c, -s, s, c);
}

float Hash21(vec2 p) {
    p = fract(p*vec2(123.34,233.53));
    p += dot(p, p+23.234);
    return fract(p.x*p.y);
}

float sdBox(vec3 p, vec3 s) {
    p = abs(p)-s;
	return length(max(p, 0.))+min(max(p.x, max(p.y, p.z)), 0.);
}

float sdTorus( vec3 p, vec2 t )
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

vec3 applyToPoint(mat4 m, vec3 v) {
    vec4 vv = vec4(v, 1.);
    vv =  m * vv;
    return vec3(vv) / vv.w;
}

vec3 applyToDirection(mat4 m, vec3 v) {
    vec4 vv = vec4(v, 0.);
    vv = m * vv;
    return normalize(vec3(vv) / vv.w);
}

float sdSphere(vec3 p, float r) {
    return length(p) - r;
}

float smin( float a, float b, float k )
{
    float res = exp2( -k*a ) + exp2( -k*b );
    return -log2( res )/k;
}

float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
  vec3 pa = p - a, ba = b - a;
  float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
  return length( pa - ba*h ) - r;
}

float GetDist(vec3 p) {
    //p = applyToPoint(inverse(uCamTransform), p);
    //float d = sdTorus(p, vec2(10, 1));
    //float d = min(
    //    sdTorus(p, vec2(10, 1)),
    //    abs(p.y + 1.)
    //);
    float d = MAX_DIST;
    const int N = 3;
    vec2 ptOnCircle = vec2(cos(uTime), sin(uTime));
    // Axis
    for (int k = 0; k < N; ++k) {
        float angle = TAU / 2. * k / N;
        vec2 axis = vec2(cos(angle), sin(angle));
        vec2 proj = dot(ptOnCircle, axis) * axis;
        d = min(d, sdTorus(p + vec3(proj.x, 0., proj.y), vec2(0.1, 0.01)));
        float r;
        float maxR = 0.015;
        float t = fract(uTime/30.+0.5);
        float m = 0.2;
        float timeIdle = 0.5 - m;
        if (t < timeIdle*0.5) {
            r = maxR;
        }
        else if (t < timeIdle*0.5 + m) {
            r = mix(maxR, 0., (t-timeIdle*0.5)/m);
        }
        else if (t < timeIdle*0.5 + m + timeIdle) {
            r = 0.;
        }
        else if (t < timeIdle*0.5 + m + timeIdle + m) {
            r = mix(0., maxR, (t-timeIdle*1.5-m)/m);
        }
        else {
            r = maxR;
        }
        if (r!=0.) {//int(floor(uTime / 6.)) % 2 == 0) {
            d = min(d, sdCapsule(p, vec3(axis.x, 0, axis.y), - vec3(axis.x, 0, axis.y), r));
        }
    }
    // Tori
    for (int k = 0; k < N; ++k) {
        float angle = TAU / 2. * k / N;
        vec2 axis = vec2(cos(angle), sin(angle));
        vec2 proj = dot(ptOnCircle, axis) * axis;
        d = min(d, sdTorus(p + vec3(proj.x, 0., proj.y), vec2(0.1, 0.01)));
    }
    //p.x += cos(uTime);
    //float d = sdSphere(p, 0.25);
    //p.x -= cos(uTime);
    //p.z += sin(uTime);
   	//d = min(d, sdSphere(p, 0.25));
    return d;
}

float RayMarch(vec3 ro, vec3 rd) {
	float dO=0.;
    
    for(int i=0; i<MAX_STEPS; i++) {
    	vec3 p = ro + rd*dO;
        float dS = GetDist(p);
        dO += dS;
        if(dO>MAX_DIST || abs(dS)<SURF_DIST) break;
    }
    
    return dO;
}

vec3 GetNormal(vec3 p) {
	float d = GetDist(p);
    vec2 e = vec2(.001, 0);
    
    vec3 n = d - vec3(
        GetDist(p-e.xyy),
        GetDist(p-e.yxy),
        GetDist(p-e.yyx));
    
    return normalize(n);
}

vec3 GetRayDir(vec2 uv, vec3 p, vec3 l, float z) {
    vec3 f = normalize(l-p),
        r = normalize(cross(vec3(0,1,0), f)),
        u = cross(f,r),
        c = f*z,
        i = c + uv.x*r + uv.y*u,
        d = normalize(i);
    return d;
}

void main()
{
    vec2 axis = vec2(1., 0.);
    vec2 ptOnCircle = vec2(cos(uTime), sin(uTime));
    vec2 proj = dot(ptOnCircle, axis) * axis;

    vec3 col = vec3(0);
    vec3 ro = uCamPos - vec3(proj.x, 0., proj.y);
    vec3 rd = normalize(
          uCamX * (vUV.x - 0.5) * uAspectRatio
        + uCamY * (vUV.y - 0.5)
        - uCamZ * uFocalLength
    );

    float d = RayMarch(ro, rd);
    
    if(d<MAX_DIST) {
    	//vec3 p = applyToPoint(inverse(uCamTransform), ro + rd * d);
        vec3 p = ro + rd * d;
    	vec3 n = GetNormal(p);
        
    	float dif = dot(n, normalize(vec3(1,2,3)))*.5+.5;
    	col += dif * vec3(0., 0.5, 0.9);
    }
    else {
        float t = 1. - abs(rd.y);
        col = mix(vec3(8.), vec3(0.2), t);
    }
    
    col = pow(col, vec3(.4545));	// gamma correction
    //col = vec3(ro);
    gl_FragColor = vec4(col,1.0);
}