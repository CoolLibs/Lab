#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/apply_color_palette.glsl"

INPUT ColorPalette Colors;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(normalized_uv);

    vec3 out_color = apply_color_palette(
        in_color, Effect_intensity,
        Colors, Colors.size // TODO(ASG) Can we pass arrays to shaders ?
    );

    out_Color = vec4(out_color, 1.);
}