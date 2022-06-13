#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform vec2 gradient_pos_a;
uniform vec2 gradient_pos_b;

uniform vec3 gradient_color_a;
uniform vec3 gradient_color_b;

uniform float gradient_dry_wet;

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 uv = _uv;
    // gradient overlay
    vec2  a_to_p         = uv - gradient_pos_a;
    vec2  a_to_b         = gradient_pos_b - gradient_pos_a;
    float atb2           = dot(a_to_b, a_to_b);
    float atp_dot_atb    = dot(a_to_p, a_to_b);
    float t              = clamp(atp_dot_atb / atb2, 0.0, 1.0);
    vec3  gradient_color = mix(gradient_color_a, gradient_color_b, t);
    vec3  tex_color      = vec3(image(uv));
    out_Color            = vec4(mix(tex_color, tex_color * gradient_color, gradient_dry_wet), 1.);
    // out_Color = vec4(image(_uv).rgr, 1.);
}