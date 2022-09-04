#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/monochrome.glsl"

INPUT RgbColor Color;
INPUT float    Brightness;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(_uv);

    vec3 color = monochrome(
        in_color, Effect_intensity,
        Color, Brightness
    );

    out_Color = vec4(color, 1.);
}