#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/color_effects_alternative.glsl"

// BEGIN DYNAMIC PARAMS

uniform RgbColor Color_coefficient;

uniform float Grey_coefficient;

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    RgbColor in_color = image(_uv);

    RgbColor out_color = color_effects_alternative(
        in_color, Effect_intensity,
        Color_coefficient, Grey_coefficient
    );

    out_Color = vec4(out_color, 1.);
}