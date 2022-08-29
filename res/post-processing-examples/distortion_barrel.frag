#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_barrel.glsl"

// default 0.5 range 0 to 1
INPUT vec2 Center; // vec2(0.5,0.5)

INPUT float distortion; // -5 to 5
// 0 forbbiden
INPUT float Scale;
INPUT float Effect_intensity;

void main()
{
    vec2 uv = _uv;
    uv /= -1; // invert
    uv *= Scale;

    vec2 out_uv    = mix(_uv, -distort(uv + Center, distortion) + Center, Effect_intensity);
    vec3 out_color = image(out_uv);

    out_Color = vec4(out_color, 1.);
}