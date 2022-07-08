#version 410

// https://www.shadertoy.com/view/lsXSWl

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float time_mod;

// END DYNAMIC PARAMS

float rng2(vec2 seed)
{
    return fract(sin(dot(seed * floor(_time * 12.), vec2(127.1, 311.7))) * 43758.5453123);
}

float rng(float seed)
{
    return rng2(vec2(seed, 1.0));
}

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 uv = _uv;

    vec2 blockS = floor(uv * vec2(24., 9.));
    vec2 blockL = floor(uv * vec2(8., 4.));

    float r     = rng2(uv);
    vec3  noise = (vec3(r, 1. - r, r / 2. + 0.5) * 1.0 - 2.0) * 0.08;

    float lineNoise = pow(rng2(blockS), 8.) * pow(rng2(blockL), 3.) - pow(rng(7.2341), 17.) * 2.;

    vec4 col1 = texture(_image, uv);
    vec4 col2 = texture(_image, uv + vec2(lineNoise * 0.05 * rng(5.), 0));
    vec4 col3 = texture(_image, uv - vec2(lineNoise * 0.05 * rng(31), 0));

    vec3 color = vec3(col1.r, col2.g, col3.b) + noise;
    out_Color  = vec4(color, 1.);
}
