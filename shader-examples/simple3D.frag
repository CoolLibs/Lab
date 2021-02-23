#version 430

const float TAU = 6.283185307179586476925286766559;

const vec3 blue = vec3(46, 155, 230) / 255;
const vec3 lightBlue = vec3(20, 99, 153) / 255;
const vec3 orange = vec3(230, 133, 54) / 255;

in vec2 vTexCoords;
uniform float uAspectRatio;
uniform vec3 uCamX;
uniform vec3 uCamY;
uniform vec3 uCamZ;
uniform vec3 uCamPos;
uniform float uFocalLength;

uniform float uTime;

#define rot2(a) mat2(cos(a), -sin(a), sin(a), cos(a))

vec3 rgb2hsv(vec3 c) {
    vec4 K = vec4(0, -1 / 3, 2 / 3, -1);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 0.000001;
    return vec3(abs(q.z + (q.w - q.y) / (6 * d + e)), d / (q.x + e), q.x);
}

// All components are in the range [0, 1], including hue.
vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1, 2 / 3, 1 / 3, 3);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0, 1), c.y);
}

struct Ray {
  vec3 origin;
  vec3 direction;
};

struct Plane {
  vec3 origin;
  vec3 normal;
};

vec2 computePlaneUv(vec3 p, Plane plane) {
  vec3 xAxis = (vec3( - plane.normal.y, plane.normal.x, 0));
  vec3 yAxis = cross(xAxis, plane.normal);

  return vec2(
    dot(xAxis, p - plane.origin),
    dot(yAxis, p - plane.origin)
  );
}

float gridOfDisks(vec2 uv, float radius, float size, float margin) {
    vec2 gridUv = fract(uv / 5.);
    vec2 gridId = floor(uv / 5.);
    if (abs(gridId.x) < size && abs(gridId.y) < size) {
      float d = distance(vec2(0.5), gridUv);
      return smoothstep(radius+margin, radius- margin, d);
    }
    return 0.;
}

float disk(vec2 uv, float radius) {
    return smoothstep(radius+0.01, radius-0.01, length(uv));
}

float circleStripe(vec2 uv, float radius, float angle) {
    float d = length(uv) - radius;
    return smoothstep(0.01, -0.01, abs(d) - 1.);
}

float sector(vec2 uv, float radius, float fraction, float width, float offset) {
    float d = length(uv) - radius;
    float a = atan(uv.y, uv.x) / TAU + 0.5;

    if (fract(a + offset) - fraction < 0.)
      return smoothstep(0.01, -0.01, abs(d) - width);
    else
      return 0.;
}

float dashedSector(vec2 uv, float radius, float fraction, float width, float offset, float enguerrand) {
  float d = length(uv) - radius;
    float a = atan(uv.y, uv.x) / TAU + 0.5;
    a = a + offset;
    a *= enguerrand;
    if (fract(a) - fraction < 0.)
      return smoothstep(0.01, -0.01, abs(d) - width);
    else
      return 0.;
}

float horloge(vec2 uv, float radius, float extension, float offset) {
    float d = length(uv) - radius;
    float a = atan(uv.y, uv.x) / TAU + 0.5 + offset;

    vec2 duv = vec2(d, fract(a * 5.));
    float dDisk = distance(duv, vec2(0., 0.5));

    return smoothstep(0.5, 0.0, abs(dDisk-extension));
}

vec3 planeUVs(Ray ray, Plane plane) {
  float t = dot(ray.origin - plane.origin, plane.normal) / dot(ray.direction, plane.normal);
  vec3 P = ray.origin + t * ray.direction;
  return vec3(
    computePlaneUv(P, plane),
    t
  );
}

vec3 scene(vec2 texCoords) {
    // Setup camera
    
    Ray ray = {
      uCamPos,
      normalize(
        uCamX * (texCoords.x - 0.5) * uAspectRatio
      + uCamY * (texCoords.y - 0.5)
      - uCamZ * uFocalLength
    )};


    vec3 col = vec3(0.);
    // vec3 blue = vec3(50, 70, 255) / 255;
    
    /*
    { // plane 0
      Plane plane = { vec3(0.,2, 0), vec3(0., 1., 0.) };
      vec3 res = planeUVs(ray, plane);
      if (res.z > 0) {
        col +=  0.5*sector(res.xy, 10, 0.2, 1, uTime*0.4) * blue;
      }
    }*/

    { // Grid of points
      Plane plane = { vec3(0., 2.5, 0.), vec3(0., 1., 0.) };
      vec3 res = planeUVs(ray, plane);
      if (res.z > 0) {
        col += gridOfDisks(res.xy, 0.15, 100, 0.015) * 0.2 * blue * smoothstep(50, 20, length(res.xy));
      }
    }
    
    { // Grid of points bis
      Plane plane = { vec3(0., 3, 0.), vec3(0., 1., 0.) };
      vec3 res = planeUVs(ray, plane);
      if (res.z > 0) {
        col += gridOfDisks(res.xy * 0.9, 0.03, 100, 0.2) * blue * smoothstep(50, 20, length(res.xy));
      }
    }

    { // Big Sector up
      for(float i = 0; i < 2; i++) {
        Plane plane = { vec3(0., 3, 0.), vec3(0., 1. + i * 0.015, 0.) };
        vec3 res = planeUVs(ray, plane);
        if (res.z > 0) {
          col +=  sector(res.xy, 10, 0.2, 1, uTime*0.3) * blue;
        }
      }
    }
    
    { // Big Sector down
      for(float i = 0; i < 2; i++) {
        Plane plane = { vec3(0., 4.5, 0.), vec3(0., 1. + i * 0.015, 0.) };
        vec3 res = planeUVs(ray, plane);
        if (res.z > 0) {
          col +=  sector(res.xy, 10, 0.2, 1, uTime*0.31) * blue;
        }
      }
    }

    { // variable sector 1
      Plane plane = { vec3(0., -2, 0.), vec3(0., 1., 0.) };
      vec3 res = planeUVs(ray, plane);
      if (res.z > 0) {
        col +=  sector(res.xy, 8, mix(0.2, 0.5, sin(uTime*0.5) * 0.5+ 0.5), 0.1, -uTime*0.3) * blue;
      }
    }
    { // variable sector 2
      Plane plane = { vec3(0., -3.5, 0.), vec3(0., 1., 0.) };
      vec3 res = planeUVs(ray, plane);
      if (res.z > 0) {
        col +=  sector(res.xy, 10, mix(0.2, 0.8, cos(uTime*0.8) * 0.5+ 0.5), 0.1, -uTime*0.3) * blue;
      }
    }

    { // variable sector 3
      Plane plane = { vec3(0., -5.5, 0.), vec3(0., 1., 0.) };
      vec3 res = planeUVs(ray, plane);
      if (res.z > 0) {
        col +=  sector(res.xy, 5, mix(0.8, 0.9, cos(-uTime*0.5) * 0.5+ 0.5), 0.1, uTime*0.2) * blue;
      }
    }

    { // cadran down
     for(float i = 0; i < 2; i++) {
        Plane plane = { vec3(0., 2+i*0.5, 0.), vec3(0., 1., 0.) };
        vec3 res = planeUVs(ray, plane);
        if (res.z > 0) {
          col += dashedSector(res.xy, 15 - 2*i, 0.15 + 0.05*i, 0.5, uTime*(0.05 - 0.06 * i), 100) * blue;
        }
     }
    }

    { // cadran top
      Plane plane = { vec3(0., 0, 0.), vec3(0., 1., 0.) };
      vec3 res = planeUVs(ray, plane);
      if (res.z > 0) {
        col += dashedSector(res.xy, 12, 0.1, 0.1, uTime*0.09, 8) * blue;
      }
    }

    { // cadran top
      Plane plane = { vec3(0., 0, 0.), vec3(0., 1., 0.) };
      vec3 res = planeUVs(ray, plane);
      if (res.z > 0) {
        col += horloge(res.xy, 5, 0.75 + 0.1 * cos(uTime*5), uTime*0.05) * vec3(255, 215, 94)/255.;
      }
    }
    
    return col;
}

void main()
{
    vec3 col = vec3(0.);
    const int N = 1 + 2 * int( 10 * length(vTexCoords - 0.5));
    for (int x = -N/2; x <= N/2; ++x) {
      for (int y = -N/2; y <= N/2; ++y) {
        //col += scene(vTexCoords + vec2(x, y)*(length(vTexCoords - 0.5))) / N / N;
        col += scene(vTexCoords + vec2(x, y)*0.001) / N / N;
      }
    }
    
    col += 0.01 * lightBlue;
    col *= clamp(1 - length(vTexCoords - 0.5) / 2, 0, 1);
    col = pow(col, vec3(0.4545)); // gamma corection

    // saturate
    col *= vec3(1, 1, 1.1);

    gl_FragColor = vec4(col, 1.);
}