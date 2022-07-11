#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/symmetry.glsl"

// BEGIN DYNAMIC PARAMS
uniform float center_x;       // 0 to 1 default 0.5
uniform float center_y;       // 0 to 1 default 0.5
uniform float angle_in_turns; // 0 to 1 (1 == 1 turn)
uniform float right_or_left;  // bool

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    vec2 in_uv = _uv;
    in_uv.x *= _aspect_ratio;

    vec2 out_uv = symmetry(
        in_uv, Effect_intensity,
        center_x, center_y, angle_in_turns, right_or_left
    );

    vec3 out_color = image(out_uv);

    out_Color = vec4(out_color, 1.);
}
