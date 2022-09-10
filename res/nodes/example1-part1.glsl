-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

                                                      INPUT Texture image;

Color image(UV uv)
{
    return image(uv);
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

                                                      float
                                                      black_and_white(RgbColor color)
{
    return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

                                                      INPUT Gradient gradient

                                                          Color
                                                      colorizer__gradient(float t)
{
    return gradient(t);
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --