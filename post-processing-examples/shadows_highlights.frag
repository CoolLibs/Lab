#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/shadows_highlights.glsl"

INPUT float factor_shadows;
INPUT float power_shadows;
INPUT float dir_shadows;
INPUT float factor_highlights;
INPUT float power_highlights;
INPUT float dir_highlights;

INPUT float Effect_intensity;

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