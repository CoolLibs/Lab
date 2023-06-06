// https://stackoverflow.com/a/26529322/15432269

// #include "_ROOT_FOLDER_/res/shader-lib/luminance.glsl"

vec3 shadows_highlights(
    vec3 in_color, float mask,
    float shadows, float highlights
)
{
    shadows    = mask * shadows + 1.;
    highlights = mask * highlights + 1.;

    float luminance = cool__luminance(in_color);

    //(shadows+1.0) changed to just shadows:
    float shadow    = clamp((pow(luminance, 1.0 / shadows) + (-0.76) * pow(luminance, 2.0 / shadows)) - luminance, 0.0, 1.0);
    float highlight = clamp((1.0 - (pow(1.0 - luminance, 1.0 / (2.0 - highlights)) + (-0.8) * pow(1.0 - luminance, 2.0 / (2.0 - highlights)))) - luminance, -1.0, 0.0);
    vec3  result    = vec3(0.0, 0.0, 0.0) + ((luminance + shadow + highlight) - 0.0) * ((in_color - vec3(0.0, 0.0, 0.0)) / (luminance - 0.0));

    // blend toward white if highlights is more than 1
    float contrastedLuminance = ((luminance - 0.5) * 1.5) + 0.5;
    float whiteInterp         = contrastedLuminance * contrastedLuminance * contrastedLuminance;
    float whiteTarget         = clamp(highlights, 1.0, 2.0) - 1.0;
    result                    = mix(result, vec3(1.0), whiteInterp * whiteTarget);

    // blend toward black if shadows is less than 1
    float invContrastedLuminance = 1.0 - contrastedLuminance;
    float blackInterp            = invContrastedLuminance * invContrastedLuminance * invContrastedLuminance;
    float blackTarget            = 1.0 - clamp(shadows, 0.0, 1.0);

    return mix(result, vec3(0.0), blackInterp * blackTarget);
}