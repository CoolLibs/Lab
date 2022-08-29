#version 410

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

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT float Intensity;

vec3 texsample(const int x, const int y, in vec2 fragCoord)
{
    vec2 uv = fragCoord.xy;
    uv      = (uv + vec2(x, y));
    return texture(_image, uv).xyz;
}

vec3 texfilter(vec2 fragCoord)
{
    vec3 sum = texsample(-1, -1, fragCoord) * -Intensity + texsample(-1, 0, fragCoord) * -Intensity + texsample(-1, 1, fragCoord) * -Intensity + texsample(0, -1, fragCoord) * -Intensity + texsample(0, 0, fragCoord) * (Intensity * 8. + 1.) // sum should always be +1
               + texsample(0, 1, fragCoord) * -Intensity + texsample(1, -1, fragCoord) * -Intensity + texsample(1, 0, fragCoord) * -Intensity + texsample(1, 1, fragCoord) * -Intensity;

    return sum;
}

void main()
{
    vec2 uv = _uv;
    // uv.x *= _aspect_ratio;

    vec3 cf = texfilter(_uv);
    // vec3 cl = texsample(0, 0, _uv);
    // vec3 cr = (u < m ? cl : cf) * l;

    out_Color = vec4(cf, 1);
}