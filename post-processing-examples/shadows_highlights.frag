#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/shadows_highlights.glsl"

input float factor_shadows;
input float power_shadows;
input float dir_shadows;
input float factor_highlights;
input float power_highlights;
input float dir_highlights;

input float Effect_intensity;

void main()
{
    RgbColor in_color = image(_uv);

    RgbColor out_color = shadows_highlights(
        in_color, Effect_intensity,
        factor_shadows, power_shadows, dir_shadows,
        factor_highlights, power_highlights, dir_highlights
    );

    out_Color = vec4(out_color, 1.);
}