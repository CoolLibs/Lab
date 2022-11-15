
float->RgbColor
{
    return vec3(input);
}

RgbColor->float
{
    return luminance(input);
}