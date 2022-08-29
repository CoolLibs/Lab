#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_barrel.glsl"

// default 0.5 range 0 to 1
INPUT vec2 Center; // vec2(0.5,0.5)

INPUT float distortion; // -5 to 5
// 0 forbbiden
INPUT float nb_tiles;

void main()
{
    vec2 uv = _uv;
    uv.x /= _aspect_ratio;
    uv /= -1; // normalize and invert

    // Put origo at the center of the viewport.
    uv += Center;

    out_Color = textureaspectCompensated(_image, -distort(uv, distortion) * nb_tiles + Center);
}