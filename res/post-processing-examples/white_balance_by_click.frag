#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/white_balance_by_click.glsl"

// Point2D
INPUT vec2 Coordinate_for_white_balance;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec3 in_color = image(_uv);

    vec3 color = white_balance_by_click(
        in_color, Effect_intensity, in_uv,
        Coordinate_for_white_balance
    );

    out_Color = vec4(color, 1.0);
}