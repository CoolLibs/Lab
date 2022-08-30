#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/color_negative_effect.glsl"

// default 1 min = 0
INPUT vec3 channels_contribution;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(_uv);

    vec3 out_color = color_negative_effet(
        in_color, Effect_intensity,
        channels_contribution
    );

    out_Color = vec4(out_color, 1.);
}