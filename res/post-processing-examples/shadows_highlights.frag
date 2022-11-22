#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/shadows_highlights.glsl"

INPUT float Shadows;
INPUT float Highlights;

INPUT float Mask;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = shadows_highlights(
        in_color, Mask,
        Shadows, Highlights
    );

    out_Color = vec4(color, 1.);
}