-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --I am UV->Color and want to become UV->Color so that's cool

                                                        INPUT Texture image;

Color gen__image(UV uv)
{
    return image(uv);
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --I am Color->float and want to become UV->float, so I need gen__image to be a UV->Color function

                                                                                                        float
                                                                                                        gen__black_and_white(RgbColor color)
{
    return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --I am float->Color and want to become UV->Color, so I need gen__black_and_white to be a UV->float function

                                                                                                            INPUT Gradient gradient

                                                                                                                Color
                                                                                                        gen__colorizer__gradient(UV uv)
{
    float t = black_and_white(uv) return gradient(t);
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --