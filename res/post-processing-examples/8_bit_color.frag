#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/8_bit_color.glsl"

INPUT float N;

INPUT float Effect_intensity;

// TODO(ASG rename ALL local `out_color` variables as `color`)

void main()
{
    vec3 in_color = image(_uv);

    vec3 color = 8_bit_color(
        in_color, Effect_intensity,
        N
    );

    out_Color = vec4(color, 1.);
}