#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/saturation.glsl"

INPUT float Saturation;

INPUT float Mask;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = saturation(
        in_color, Mask,
        Saturation
    );

    out_Color = vec4(color, 1.);
}
