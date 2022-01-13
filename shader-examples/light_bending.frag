#version 430
//Thanks to The Art of Code

layout(location = 0) in vec2       _uv;
uniform float _time;
out vec4      out_Color;

#include "_COOL_RES_/shaders/camera.glsl"

#define MAX_STEPS 100
#define MAX_DIST  100.
#define SURF_DIST .001

// BEGIN DYNAMIC PARAMS

uniform float IOR; //5 max

// END DYNAMIC PARAMS

float sdf(vec3 p)
{
    vec3 q = abs(p) - 1.;  
	return length(max(q, 0.)) + min(max(q.x, max(q.y, q.z)), 0.);
}

struct RayMarchRes {
    float dist;
    float pas;
};

RayMarchRes ray_march(vec3 ro, vec3 rd, float side)
{
    float dO = 0.;
    int i = 0;

    for (i; i < MAX_STEPS; i++) {
        vec3  p  = ro + rd * dO;
        float dS = sdf(p) * side;
        dO += dS;
        if (dO > MAX_DIST || abs(dS) < SURF_DIST) {
            break;
        }
    }

    return RayMarchRes(dO,i);
}

vec3 normal(vec3 p)
{
    float      d = sdf(p);
    const vec2 e = vec2(0.001, 0.);

    vec3 n = d - vec3(
                     sdf(p - e.xyy),
                     sdf(p - e.yxy),
                     sdf(p - e.yyx));

    return normalize(n);
}

vec3 render(vec3 ro, vec3 rd)
{
    RayMarchRes res = ray_march(ro, rd, 1.); //Outside
    vec3  col = vec3(.000, 0.0711, 0.0949);
    float d = res.dist;
    float pas = res.pas;

    if (d < MAX_DIST) {
        vec3 p = ro + rd * d;
        vec3 norm = normal(p);
        vec3 refletOut = reflect(rd, norm); 
        vec3 refractionIn = refract(rd,norm,1./IOR);

        vec3 pEnter = p - norm * SURF_DIST * 3.;
        RayMarchRes dIn = ray_march(pEnter,refractionIn, -1.); // Inside
        float d2 = dIn.dist;

        vec3 pExit = pEnter + refractionIn * d2;
        vec3 normExit = -normal(pExit);

        vec3 reflectText = vec3(0);
        vec3 refractionOut = vec3(0);
        float abb = .01;
        //Red
        refractionOut = refract(refractionIn,normExit,IOR-abb);
        if(dot(refractionOut, refractionOut) == 0.){
            refractionOut = reflect(refractionIn, normExit);
        }
        reflectText.r = refractionOut.r;
        //Green
        refractionOut = refract(refractionIn,normExit,IOR);
        if(dot(refractionOut, refractionOut) == 0.){
            refractionOut = reflect(refractionIn, normExit);
        }
        reflectText.g = refractionOut.g;
        //Blue
        refractionOut = refract(refractionIn,normExit,IOR+abb);
        if(dot(refractionOut, refractionOut) == 0.){
            refractionOut = reflect(refractionIn, normExit);
        }
        reflectText.b = refractionOut.b;

        float dens = .1;
        float optDist = exp(-d2*dens);

        reflectText= reflectText * optDist;

        float fresnelRefl = pow(1.+dot(rd, norm),5.);

        col = mix(reflectText, refletOut, fresnelRefl);
    }
    col = pow(col, vec3(.4545)); // gamma correction
    return col;
}

void main()
{
    vec3 ro = cool_ray_origin();
    vec3 rd = cool_ray_direction();

    out_Color = vec4(render(ro, rd), 1.);
}