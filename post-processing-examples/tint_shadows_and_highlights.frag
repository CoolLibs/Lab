#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/tint_shadows_and_highlights.glsl"

// BEGIN DYNAMIC PARAMS

uniform RgbColor Color_shadows;
uniform RgbColor Color_highlights;

uniform float Shadows_intensity;
uniform float Highlights_intensity;

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    RgbColor in_color = image(_uv);

    RgbColor out_color = tint_shadows_and_highlights(
        in_color, Effect_intensity,
        Color_shadows, Color_highlights, Shadows_intensity, Highlights_intensity
    );

    out_Color = vec4(out_color, 1.);
}