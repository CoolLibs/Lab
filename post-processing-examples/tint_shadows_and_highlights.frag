#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/tint_shadows_and_highlights.glsl"

// BEGIN DYNAMIC PARAMS

uniform vec3 Color_shadows;
uniform vec3 Color_highlights;

uniform float Shadows_intensity;
uniform float Highlights_intensity;

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    vec3 in_color = image(_uv);

    vec3 out_color = tint_shadows_and_highlights(
        in_color, Effect_intensity,
        Color_shadows, Color_highlights, Shadows_intensity, Highlights_intensity
    );

    out_Color = vec4(out_color, 1.);
}