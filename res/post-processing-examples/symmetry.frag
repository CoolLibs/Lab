#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/symmetry.glsl"

// 0 to 1 default 0.5
// Point2D
INPUT vec2  Center;
INPUT Angle Angle_symmetry;
INPUT bool  Invert_symmetry;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = symmetry(
        in_uv, Effect_intensity,
        Center, Angle_symmetry, Invert_symmetry
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}
