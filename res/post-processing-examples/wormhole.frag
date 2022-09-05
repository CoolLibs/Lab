#version 410

// #include "_COOL_RES_/shaders/math.glsl"

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/wormhole.glsl"

INPUT float   Atractivity; // default 0.008
INPUT float   Symmetry;    // min = 0 max = 0.5
INPUT Point2D Center;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = wormhole(
        in_uv, Effect_intensity,
        Center, Atractivity, Symmetry
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}
