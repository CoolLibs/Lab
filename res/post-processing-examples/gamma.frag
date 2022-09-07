#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/gamma.glsl"

INPUT float Gamma;
INPUT bool  Clamp; /// Clamps the color between 0 and 1 before applying the Gamma power.

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = gamma_modifier(
        in_color, Effect_intensity,
        Gamma, Clamp
    );

    out_Color = vec4(color, 1.);
}