#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/symmetry_star.glsl"

INPUT float Size;           // 0 forbidden 0.001 to 0.5
INPUT Angle Angle_in_turns; // 0 to 1 (1 == 1 turn)

INPUT int Nb_iterations;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv - 0.5;
    in_uv.x *= _aspect_ratio;

    vec2 out_uv = symmetry_star(
        in_uv, Effect_intensity,
        Size, Angle_in_turns, Nb_iterations
    );

    out_uv.x /= _aspect_ratio;
    out_uv += 0.5;

    vec3 out_color = image(out_uv).xyz;

    out_Color = vec4(out_color, 1.);
}