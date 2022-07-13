#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/shadows_highlights.glsl"

// BEGIN DYNAMIC PARAMS

uniform float factor_shadows;
uniform float power_shadows;
uniform float dir_shadows;
uniform float factor_highlights;
uniform float power_highlights;
uniform float dir_highlights;

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    vec3 in_color = image(_uv);

    vec3 out_color = shadows_highlights(
        in_color, Effect_intensity,
        factor_shadows, power_shadows, dir_shadows,
        factor_highlights, power_highlights, dir_highlights
    );

    out_Color = vec4(out_color, 1.);
}