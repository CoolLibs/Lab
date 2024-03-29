#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/golden_noise.glsl"

INPUT float Seed;

INPUT float Mask;

void main()
{
    vec2 in_uv = _uv + 1.;
    in_uv.x *= _aspect_ratio;

    // vec2 out_uv = golden_noise_uv(
    //     in_uv, Mask,
    //     Seed
    // );

    // vec3 color = image(out_uv);

    vec3 color = golden_noise_color(
        in_uv, Mask,
        Seed
    );

    out_Color = vec4(color, 1.);
}