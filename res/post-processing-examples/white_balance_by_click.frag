#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/white_balance_by_click.glsl"

INPUT Point2D Coordinate_for_white_balance;

INPUT float Effect_intensity;

void main()
{
    vec2 uv       = normalized_uv();
    vec3 in_color = image(uv);

    vec3 color = white_balance_by_click(
        in_color, Effect_intensity, uv,
        Coordinate_for_white_balance
    );

    out_Color = vec4(color, 1.0);
}