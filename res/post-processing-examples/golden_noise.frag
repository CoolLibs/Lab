#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/golden_noise.glsl"

INPUT float Seed; // default 48

INPUT float Effect_intensity;

void main()
{
    vec3 image = image(_uv);
    vec2 uv    = _uv;
    uv.x *= _aspect_ratio;

    vec3 out_color = vec3(
        golden_noise(uv, Seed + 0.1),
        golden_noise(uv, Seed + 0.2),
        golden_noise(uv, Seed + 0.3)
    );

    out_Color = vec4(out_color, 1.);
}