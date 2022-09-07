#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/glitch.glsl"

INPUT float Time;
INPUT vec2  Squares_size;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    float glitch = glitch(
        in_uv,
        Squares_size, Time
    );
    vec3 col1 = image(in_uv);
    vec3 col2 = image(in_uv + vec2(glitch * Effect_intensity * rng(5., Time), 0.));
    vec3 col3 = image(in_uv - vec2(glitch * Effect_intensity * rng(31, Time), 0.));

    vec3 color = vec3(col1.r, col2.g, col3.b);

    out_Color = vec4(color, 1.);
}
