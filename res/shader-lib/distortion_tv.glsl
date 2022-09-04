
// https://www.shadertoy.com/view/XtlXzj

float terrain(
    float Effect_intensity,
    float x, int Nb_iteration, float Strength
)
{
    // Used Shadershop
    // return ((sin( (sin( (x - -1.33) / 0.76 / 1.23 ) * 0.8 - 0.69) / 0.58 )) * (((((sin( (((x - -1.33) / 0.76 - -3.0) / 2.61 - -0.38) / 1.52 ) * 2.25) * (sin( (((x - -1.33) / 0.76 - -3.0) / 2.61 - -0.47) / 1.61 ) * 1.03))) * (sin( ((x - -1.33) / 0.76 - -3.0) / 2.61 / 0.44 ) * 1.48)) * 1.08)) * 0.78;
    // You actually should look after sigma.
    float v = 0.;
    x *= 3.;
    for (float n = 0.; n < Nb_iteration; n++)
    {
        v += ((sin((x * sin(n / 2.142)) + (n / 1.41))) / Nb_iteration) * Strength * (Effect_intensity + 1.);
    }
    return pow(v, 3.);
}
