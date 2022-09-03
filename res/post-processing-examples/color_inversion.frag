#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/color_inversion.glsl"

// default 1 min = 0
INPUT vec3 Channels_contribution;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(_uv);

    vec3 color = color_inversion(
        in_color, Effect_intensity,
        Channels_contribution
    );

    out_Color = vec4(color, 1.);
}