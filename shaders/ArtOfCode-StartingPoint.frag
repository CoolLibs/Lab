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

varying vec2 vTexCoords;
uniform float uAspectRatio;
//uniform mat4 uCamTransform;
uniform vec3 camPos;
uniform vec3 camX;
uniform vec3 camY;
uniform vec3 camZ;
uniform float uFocalLength;

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

float GetDist(vec3 p) {
    //p = applyToPoint(inverse(uCamTransform), p);
    float d = sdTorus(p, vec2(10, 1));
    //float d = min(
    //    sdTorus(p, vec2(10, 1)),
    //    abs(p.y + 1.)
    //);
    //float d = sdBox(p, vec3(1));
   	
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
    //vec2 uv = vTexCoords - vec2(0.5);
    //uv.x *= uAspectRatio;
    vec2 uv = vTexCoords;
    
    vec3 col = vec3(0);
    vec3 ro = camPos;
    vec3 cBL = ro - uFocalLength * camZ;
    vec3 cBR = cBL + uAspectRatio * camX;
    vec3 cTL = cBL + camY;
    vec3 rd = normalize(cBL + uv.x * (cBR - cBL) + uv.y * (cTL - cBL) - ro);
    //ro = applyToPoint    (uCamTransform, ro);
    //rd = applyToDirection(uCamTransform, rd);

    float d = RayMarch(ro, rd);
    
    if(d<MAX_DIST) {
    	//vec3 p = applyToPoint(inverse(uCamTransform), ro + rd * d);
        vec3 p = ro + rd * d;
    	vec3 n = GetNormal(p);
        
    	float dif = dot(n, normalize(vec3(1,2,3)))*.5+.5;
    	col += dif;
    }
    else {
        col = vec3(1, 0, 0);
    }
    
    col = pow(col, vec3(.4545));	// gamma correction
    //col = vec3(ro);
    gl_FragColor = vec4(col,1.0);
}