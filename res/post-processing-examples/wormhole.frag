#version 410

// https://www.shadertoy.com/view/tdXfW7

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT float Atractivity; // default 0.008
INPUT float Symmetry;    // min = 0 max = 0.5
INPUT float Intensity;
INPUT float Wormhole_smooth; // fobidden 0
INPUT float Wormhole_size;   // fobidden 0

INPUT RgbColor Wormhole_color;

void main()
{
    vec2 p = _uv - 0.5;
    p.x *= _aspect_ratio;

    vec2  tuv   = vec2(atan(p.x, p.y) / PI * Symmetry, Atractivity / length(p)) + .5;
    float l     = sqrt(p.x * p.x + p.y * p.y);
    vec3  color = vec3(0.);
    if (l > 2.)
    {
        l = Wormhole_size;
    }
    else
    {
        l = l * Wormhole_smooth - Wormhole_size;
        l = -(l * l) + Wormhole_size;
    }
    tuv = abs(1. - 2. * tuv);
    color += Wormhole_color;
    color += image(tuv).rgb * l * Intensity;
    out_Color = vec4(color, 1.);
}
