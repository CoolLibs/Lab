// https://www.shadertoy.com/view/NsS3Dt

// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"

vec3 saturation(vec3 in_color, float mask, float sat)
{
    float grey = cool__luminance(in_color);
    return mix(vec3(grey), in_color, sat * mask + 1.);
}