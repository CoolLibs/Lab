#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"

INPUT int      Number_of_colors; // min 2
INPUT Gradient gradient;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(normalized_uv());

    float luminance = luminance(in_color);
    // TODO(ASG) Move this in a threshold file
    luminance = clamp(luminance, 0., 0.999); // Make sure that pure white doesn't get mapped to a range of its own by the floor()
    luminance *= (Number_of_colors);
    luminance = floor(luminance);
    luminance /= (Number_of_colors - 1);
    // TODO end of theshold fnuction

    vec3 color = gradient(luminance).rgb;

    color = mix(in_color, color, Effect_intensity);

    out_Color = vec4(color, 1.);
}