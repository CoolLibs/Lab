#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/golden_noise.glsl"

INPUT float Seed; // default 48

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    // vec2 out_uv = golden_noise_uv(
    //     in_uv, Effect_intensity,
    //     Seed
    // );

    // vec3 color = image(out_uv);

    vec3 color = golden_noise_color(
        in_uv, Effect_intensity,
        Seed
    );

    out_Color = vec4(color, 1.);
}