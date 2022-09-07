// https://www.shadertoy.com/view/MtX3z2

float Sigmoid(float x, float strength)
{
    return 1.0 / (1.0 + (exp(-(x - 0.5) * strength)));
}

vec3 sigmoid_contrast(
    vec3 in_color, float effect_intensity,
    float strength
)
{
    return mix(
        in_color,
        vec3(
            Sigmoid(in_color.r, strength),
            Sigmoid(in_color.g, strength),
            Sigmoid(in_color.b, strength)
        ),
        effect_intensity
    );
}