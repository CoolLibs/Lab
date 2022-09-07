#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/vignetting.glsl"

INPUT float    Strength;
INPUT float    Radius;
INPUT RgbColor Color;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3 color = vignetting(
        in_color, Effect_intensity, _uv, _aspect_ratio,
        Strength, Radius, Color
    );

    out_Color = vec4(color, 1.);
}