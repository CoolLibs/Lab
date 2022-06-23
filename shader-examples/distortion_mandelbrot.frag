#version 430

// https://www.shadertoy.com/view/MsXXD7

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float size;     // negative values to flip the pattern
uniform float offset_x; // default .5 min = 0 max = 1
uniform float offset_y; // default .5 min = 0 max = 1
uniform float time_mod; // min = 0 max = 20

uniform int nb_iteration; // min = 1

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 offset = vec2(offset_x, offset_y);

    vec2 C = (size * (_uv - offset));
    // position better to see the fractal
    C      = -C.yx * 1.8 + vec2(-.5, 0);
    vec2 Z = vec2(0);

    float m = float(nb_iteration);
    float n = fract(.3 * time_mod / m) * m;
    Z       = C * fract(n);
    for (int i = 0; i < nb_iteration; i++) {
        if (float(i) > n)
            break;
        Z = vec2(Z.x * Z.x - Z.y * Z.y, 2.0 * Z.x * Z.y) + C;
    }

    // image map
    vec3 color = image(-Z.yx * .3 + .5).rgb;

    out_Color = vec4(color, 1.);
}
