#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/color_effects_alternative.glsl"

// BEGIN DYNAMIC PARAMS

uniform vec3 Color_coefficient;

uniform float Grey_coefficient;

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    vec3 in_color = image(_uv);

    vec3 out_color = color_effects_alternative(
        in_color, Effect_intensity,
        Color_coefficient, Grey_coefficient
    );

    out_Color = vec4(out_color, 1.);
}