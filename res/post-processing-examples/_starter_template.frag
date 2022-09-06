#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

void main()
{
    vec2 in_uv = normalized_uv();

    vec3 color = image(in_uv).rgr;

    out_Color = vec4(color, 1.);
}