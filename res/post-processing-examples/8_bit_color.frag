#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/8_bit_color.glsl"

INPUT int N;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(_uv);

    vec3 color = bit_color(
        in_color, Effect_intensity,
        float(N)
    );

    out_Color = vec4(color, 1.);
}