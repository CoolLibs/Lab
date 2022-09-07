// https://www.shadertoy.com/view/MtX3z2

float Sigmoid(float x)
{
    return 1.0 / (1.0 + (exp(-(x - 0.5) * Strength)));
}

vec3 sigmoid_contrast(
    vec3 in_color, float effect_intensity,
    float strength
)
{
    return mix(
        in_color,
        vec3(
            Sigmoid(in_color.r),
            Sigmoid(in_color.g),
            Sigmoid(in_color.b)
        ),
        effect_intensity
    )
}