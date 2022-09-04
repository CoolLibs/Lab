#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/jacobian_undistortion.glsl"

INPUT float Time_mod;
INPUT float Resolution_bias;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = jacobian_undistortion(
        in_uv, Effect_intensity,
        _aspect_ratio,
        Time_mod, Resolution_bias
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}