#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _texture;

// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_by_texture.glsl"

INPUT float Strength;

INPUT float Mask;

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = distortion_by_texture(
        in_uv, Mask, _texture,
        Strength
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}