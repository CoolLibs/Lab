#version 410

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/swirl_distortion.glsl"

// default 0.5
// Point2D
INPUT vec2  Center;
INPUT float Size; // only positive values

INPUT float Effect_intensity; // positive and negative values

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = swirl_distortion(
        in_uv, Effect_intensity,
        Center, Size
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}