#version 410

// https://www.shadertoy.com/view/WdK3zt

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT float time_mod;
INPUT float scale;  // default 0.125
INPUT float offs_x; // default .5
INPUT float offs_y; // default .5

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