#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_mandelbrot.glsl"

INPUT float Scale; // negative values to flip the pattern
                   // default .5 min = 0 max = 1 // Point2D
INPUT vec2 Center;
// 0 forbidden
INPUT float Subdivision; // min = 0 max = 20

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = distortion_mandelbrot(
        in_uv, Effect_intensity,
        Center, Scale, Subdivision
    );

    vec3 out_color = image(out_uv);

    out_Color = vec4(out_color, 1.);
}
