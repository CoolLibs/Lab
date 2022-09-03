#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/threshold.glsl"

INPUT int Number_of_colors; // min 2

INPUT RgbColor Color1;
INPUT RgbColor Color2;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(_uv);

    vec3 out_color = threshold(
        in_color, Effect_intensity,
        Number_of_colors, Color1, Color2
    );

    out_Color = vec4(out_color, 1.);
}