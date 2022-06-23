// Color temperature (sRGB) stuff
// Copyright (C) 2014 by Benjamin 'BeRo' Rosseaux
// Because the german law knows no public domain in the usual sense,
// this code is licensed under the CC0 license
// http://creativecommons.org/publicdomain/zero/1.0/

#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

const float PI2 = 6.2831853071;

// Valid from 1000 to 40000 K (and additionally 0 for pure full white)
vec3 colorTemperatureToRGB(const in float temperature)
{
    // Values from: http://blenderartists.org/forum/showthread.php?270332-OSL-Goodness&p=2268693&viewfull=1#post2268693
    mat3 m = (temperature <= 6500.0) ? mat3(vec3(0.0, -2902.1955373783176, -8257.7997278925690),
                                            vec3(0.0, 1669.5803561666639, 2575.2827530017594),
                                            vec3(1.0, 1.3302673723350029, 1.8993753891711275))
                                     : mat3(vec3(1745.0425298314172, 1216.6168361476490, -8257.7997278925690),
                                            vec3(-2666.3474220535695, -2173.1012343082230, 2575.2827530017594),
                                            vec3(0.55995389139931482, 0.70381203140554553, 1.8993753891711275));
    return mix(clamp(vec3(m[0] / (vec3(clamp(temperature, 1000.0, 40000.0)) + m[1]) + m[2]), vec3(0.0), vec3(1.0)), vec3(1.0), smoothstep(1000.0, 0.0, temperature));
}

// BEGIN DYNAMIC PARAMS

uniform float temperature; // default 1000 to 40000
uniform float temperatureStrength;
uniform float LuminancePreservationFactor;

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    // vec3 color_pick = texture(iChannel0,-iMouse.xy/iResolution.xy).rgb;
    vec2 uv = _uv;
    uv.x *= _aspect_ratio;
    float temperature_in_kelvins = mix(1000, 40000, temperature); // because our parameter goes from 0 to 1
    vec3  inColor                = image(_uv).xyz;
    vec3  outColor               = mix(inColor, inColor * colorTemperatureToRGB(temperature_in_kelvins), temperatureStrength);
    outColor *= mix(1.0, dot(inColor, vec3(0.2126, 0.7152, 0.0722)) / max(dot(outColor, vec3(0.2126, 0.7152, 0.0722)), 1e-5), LuminancePreservationFactor);
    out_Color = vec4(outColor, 1.0);
}