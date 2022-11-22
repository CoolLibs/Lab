#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/black_and_white.glsl"

INPUT vec3 Channels_contribution;
INPUT bool Normalize; /// Makes sure the sum of "Channels contribution" is 1

INPUT float Mask;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = black_and_white(
        in_color, Mask,
        Channels_contribution, Normalize
    );

    out_Color = vec4(color, 1.);
}
