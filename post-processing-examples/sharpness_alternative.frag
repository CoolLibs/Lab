#version 430

// from https://www.shadertoy.com/view/llXGWf

// Sharpness kernel
// -x   -x    -x
// -x  x*8+1  -x
// -x   -x    -x

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"

// BEGIN DYNAMIC PARAMS

uniform float Offset_strength;
uniform float Sharpening_strength;

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 uv = _uv;
    uv.x *= _aspect_ratio;

    // Time varying pixel color
    RgbColor col = image(_uv).rgb;

    RgbColor blur = col;
    blur += texture(_image, _uv + vec2(0.0, 0.001 * Offset_strength)).rgb;
    blur += texture(_image, _uv + vec2(0.001 * Offset_strength, 0.0)).rgb;
    blur += texture(_image, _uv - vec2(0.0, 0.001 * Offset_strength)).rgb;
    blur += texture(_image, _uv - vec2(0.001 * Offset_strength, 0.0)).rgb;

    blur += texture(_image, _uv + vec2(0.001 * Offset_strength)).rgb / 2.0;
    blur += texture(_image, _uv - vec2(0.001 * Offset_strength)).rgb / 2.0;
    blur += texture(_image, _uv + vec2(0.001 * Offset_strength, -0.001 * Offset_strength)).rgb / 2.0;
    blur += texture(_image, _uv + vec2(-0.001 * Offset_strength, 0.001 * Offset_strength)).rgb / 2.0;

    blur /= 7.0;

    float sharpness = (col - blur).r * Sharpening_strength;

    // col = RgbColor(0);
    // col -= step(distance(fract(uv.x + (sin(_time) + 1.0) / 2.0) + 0.5, 0.5) * 2.0, 0.013);

    if (floor(uv.x + ((sin(_time) + 1.0) / 2.0)) > 0.0)
    {
        col += sharpness;
    }

    out_Color = vec4(col, 1.0);
}