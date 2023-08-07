#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/tint.glsl"

INPUT float Tint;

INPUT float Mask;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = tint(
        in_color, Mask,
        Tint
    );

    out_Color = vec4(color, 1.);
}