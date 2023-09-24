
// LIGHT PART OF SMOKE EFFECT (WIP)

#define LIGHT_ATTENUATION_FACTOR     1.65
#define NUM_LIGHT_COLORS             3
#define NUM_LIGHTS                   3
#define UNIFORM_LIGHT_SPEED          1
#define MAX_VOLUME_LIGHT_MARCH_STEPS 4

struct OrbLight {
    vec3  Position;
    float Radius;
    vec3  LightColor;
};

vec3 GetAmbientLight()
{
    return 0.7 * vec3(1, 1, 1);
}

float Luminance(vec3 color)
{
    return (color.r * 0.3) + (color.g * 0.59) + (color.b * 0.11);
}

bool IsColorInsignificant(vec3 color)
{
    const float minValue = 0.009;
    return Luminance(color) < minValue;
}

vec3 GetLightColor(int lightIndex)
{
    return vec3(1.0, 1.0, 1.0);
}

OrbLight GetLight(int lightIndex)
{
    // Create an orb of the selected Index
    const float lightMultiplier = 17.0f;
#if UNIFORM_LIGHT_SPEED
    float theta  = _time * 0.7 + float(lightIndex) * 3.1415926 * 2.0 / float(NUM_LIGHT_COLORS);
    float radius = 18.5f;
#else
    float theta  = _time * 0.4 * (float(lightIndex) + 1.0f);
    float radius = 19.0f + float(lightIndex) * 2.0;
#endif

    OrbLight orb;
    orb.Position   = vec3(radius * cos(theta), 6.0 + sin(theta * 2.0) * 2.5, radius * sin(theta));
    orb.LightColor = GetLightColor(lightIndex) * lightMultiplier;
    orb.Radius     = 0.8f;

    return orb;
}

float GetLightAttenuation(float distanceToLight)
{
    // Get the atenuation of the light from a distance
    return 1.0 / pow(distanceToLight, LIGHT_ATTENUATION_FACTOR);
}
