///-- USER'S DOC --
/// At the grey coefficient given, we replace all pixels at this luminosity by the desired color
/// The rest of the grey levels are mapped automatially according to a power curve.

///-- DEV'S DOC --
/// We use log functions to adapt the color coefficient to the grey coefficient.
/// We just need to choose the desired color and the pow will be automatiquely set to adapt.
/// Initial function was grey^r = color; so, r = log(color)/log(grey).
/// With grey the luminosity factor and r the power's exponent.

// #include "_ROOT_FOLDER_/shader-lib/luminance.glsl"

RgbColor color_effects(
    RgbColor in_color, float effect_intensity,
    RgbColor color_coefficient, float grey_coefficient, bool one_color_mode
)
{
    RgbColor image = in_color;
    if (one_color_mode)
    {
        image = RgbColor(luminance(image));
    }
    RgbColor out_color = pow(image, log(color_coefficient) / log(grey_coefficient));
    return mix(in_color, out_color, effect_intensity);
}