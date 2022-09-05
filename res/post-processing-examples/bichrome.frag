#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/bichrome.glsl"

INPUT RgbColor Color1;
INPUT RgbColor Color2;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = bichrome(
        in_color, Effect_intensity,
        Color1, Color2
    );

    out_Color = vec4(color, 1.);
}