#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/white_balance_by_click.glsl"

INPUT vec2 Coordinate_for_white_balance;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;
    in_uv.x *= _aspect_ratio;

    RgbColor in_color = image(_uv);

    RgbColor out_color = white_balance_by_click(
        in_color, Effect_intensity, in_uv,
        Coordinate_for_white_balance
    );

    out_Color = vec4(out_color, 1.0);
}