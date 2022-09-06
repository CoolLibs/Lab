#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT ColorPalette Colors;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(normalized_uv());

    vec3  col      = Colors(0);
    float min_dist = distance(in_color, Colors(0));
    for (int i = 1; i < Colors_length; i++)
    {
        float dist = distance(in_color, Colors(i));
        if (dist < min_dist)
        {
            min_dist = distance(in_color, Colors(i));
            col      = Colors(i);
        }
    }
    vec3 color = col;
    color      = mix(in_color, color, Effect_intensity);

    out_Color = vec4(color, 1.);
}