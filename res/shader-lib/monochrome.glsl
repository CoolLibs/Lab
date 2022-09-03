///-- DEV'S DOC --
/// All colors with the given `target_luminance` are replaced with `color`.
/// The rest of the colors are mapped automatically according to a power curve.
/// The function is of the form `luminance^x = color`, so `x = log(color) / log(target_luminance)`.

// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"

vec3 monochrome_impl(float luminance, vec3 color, float target_luminance)
{
    return pow(
        vec3(luminance),
        log(color) / log(vec3(target_luminance))
    );
}

vec3 monochrome(
    vec3 in_color, float effect_intensity,
    vec3 color, float brightness
)
{
    vec3 out_color = monochrome_impl(luminance(in_color), color, 1. - brightness);
    return mix(in_color, out_color, effect_intensity);
}