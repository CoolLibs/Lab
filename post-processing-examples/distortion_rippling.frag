#version 430

// https://www.shadertoy.com/view/MlsyzB

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"

input float time_mod;
input float thickness; // default .1
input float offset_x;  // default .5 min = 0 max = 1
input float offset_y;  // default .5 min = 0 max = 1

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 offset = vec2(offset_x, offset_y);

    vec2 uv = _uv;
    uv -= offset;

    vec4 color = texture(_image, uv + offset);

    uv.x *= _aspect_ratio;
    float d = length(uv);
    uv.x *= _aspect_ratio;

    float radius = time_mod * 0.25;
    // float thickness = 0.1; // + (iTime * 0.02); // womp-womp

    float blend   = smoothstep(radius - thickness, radius - thickness - 0.05, d);
    float distort = cos(((d - (radius - thickness)) / thickness) * (PI / 2.0));

    // Try adding any of these for some extra flavor
    // distort /= cos(distort); 			// 1
    // distort /= distort * cos(distort);	// 2
    // distort /= sin(distort);				// 3
    // distort /= distort * sin(distort);	// 4
    // distort += distort;					// 5

    color = mix(color, texture(_image, (uv * distort) + offset), blend);

    out_Color = color;
}