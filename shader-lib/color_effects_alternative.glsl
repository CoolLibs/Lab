// #include "_ROOT_FOLDER_/shader-lib/luminance.glsl"

float pow_factor(float a, float x)
{
    return pow(x, a);
}

float red_transformation(float red, vec3 color_coefficient, float grey_coefficient)
{
    return pow_factor(log(color_coefficient.r) / log(grey_coefficient), red);
}

float green_transformation(float green, vec3 color_coefficient, float grey_coefficient)
{
    return pow_factor(log(color_coefficient.g) / log(grey_coefficient), green);
}

float blue_transformation(float blue, vec3 color_coefficient, float grey_coefficient)
{
    return pow_factor(log(color_coefficient.b) / log(grey_coefficient), blue);
}

RgbColor color_transformation(RgbColor image, vec3 color_coefficient, float grey_coefficient)
{
    float luminosity = luminance(image);
    return RgbColor(red_transformation(luminosity, color_coefficient, grey_coefficient), green_transformation(luminosity, color_coefficient, grey_coefficient), blue_transformation(luminosity, color_coefficient, grey_coefficient));
}

RgbColor color_effects_alternative(
    RgbColor in_color, float effect_intensity,
    RgbColor color_coefficient, float grey_coefficient
)
{
    RgbColor out_color = color_transformation(in_color, color_coefficient, grey_coefficient);
    return mix(in_color, out_color, effect_intensity);
}