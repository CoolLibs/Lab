#version 430

// https://www.shadertoy.com/view/WlByRW

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"

input float time_mod;
input float resolution_bias;

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 uv = _uv;
    uv.x *= _aspect_ratio;
    vec2 R = gl_FragCoord.xy / uv * resolution_bias;
    vec2 U = (2. * _uv * R - R) / R.y; // normalized coordinates

    U += .5 * U * mat2(cos(2. * U + time_mod), sin(2. * U - time_mod)); // distorted coordinates
    U *= 4.;

    vec4 O = vec4(0.);

    mat2 J = transpose(mat2(dFdx(U), dFdy(U))) * R.y / 8.; // the Jacobian store the local distorted frame

    // mat2 J = mat2(dFdx(U), dFdy(U)) * R.y / 8.; // the Jacobian store the local distorted frame

    RgbColor color = image(_uv * J).rgb;
    out_Color      = vec4(color, 1.);
}