#version 430

// https://www.shadertoy.com/view/WdK3zt

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"

// BEGIN DYNAMIC PARAMS

uniform float time_mod;
uniform float scale;  // default 0.125
uniform float offs_x; // default .5
uniform float offs_y; // default .5

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    float tme = time_mod * .5;
    // float scale = 0.125;

    vec2  adjVec = vec2((sin(tme) * .5), (cos(tme) * .5));
    vec2  uv     = _uv;
    vec2  uvM;
    float posMod;
    vec2  offset = vec2(offs_x, offs_y);

    uvM = mod(uv + normalize(uv - offset) * adjVec, scale);
    uv  = normalize(uvM - vec2(scale / 2.)) * (adjVec + vec2(.5));

    if (mod(floor((uv.x) / scale), 2.) == 0.)
    {
        uvM.x = scale - uvM.x;
    }
    if (mod(floor((uv.y) / scale), 2.) == 0.)
    {
        uvM.y = scale - uvM.y;
    }

    uvM += adjVec * length(uvM + adjVec - vec2(0.5));
    out_Color = vec4(image(uvM).rgb, 1.);
}