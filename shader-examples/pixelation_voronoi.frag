#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float Size;
uniform float Speed;
uniform float Movement;
uniform float Time_mode;
uniform float Distance_mode;

// END DYNAMIC PARAMS

vec3 image(vec2 uv)
{
    return texture2D(_image, uv).rgb;
}

vec2 N22(vec2 p)
{
    vec3 a = fract(p.xyx * vec3(123.34, 234.34, 345.65));
    a += dot(a, a + 34.45);
    return fract(vec2(a.x * a.y, a.y * a.z));
}

void main()
{
    vec2 uv = _uv;
    uv.x *= _aspect_ratio;

    float m = 0.;
    float t = 0;
    if (Time_mode > .5) {
        t = _time * Speed;
    }
    else {
        t = Movement;
    }

    float minDist   = 100.;
    float cellIndex = 0.;

    vec3 col = vec3(0);

    if (false) {
        for (float i = 0.; i < 50.; i++) {
            vec2 n = N22(vec2(i));
            vec2 p = sin(n * t);

            float d = length(uv - p);
            m += smoothstep(.02, .01, d);

            if (d < minDist) {
                minDist   = d;
                cellIndex = i;
            }
        }
        col = vec3(minDist);
    }
    else {
        uv *= Size;
        vec2 gv  = fract(uv) - .5;
        vec2 id  = floor(uv);
        vec2 cid = vec2(0);

        for (float y = -1.; y <= 1.; y++) {
            for (float x = -1.; x <= 1.; x++) {
                vec2 offs = vec2(x, y);

                vec2 n = N22(id + offs);
                vec2 p = offs + sin(n * t) * .5;
                p -= gv;
                float d = pow(
                    pow(abs(p.x), Distance_mode) + pow(abs(p.y), Distance_mode),
                    1 / Distance_mode);

                if (d < minDist) {
                    minDist = d;
                    cid     = id + offs;
                }
            }
        }
        vec2 image_uv = cid / Size;
        image_uv.x /= _aspect_ratio;
        col = image(image_uv);
    }

    out_Color = vec4(col, 1.);
}