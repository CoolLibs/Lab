// https://www.shadertoy.com/view/WlByRW

vec2 jacobian_undistortion(
    vec2 in_uv, float effect_intensity,
    float aspect_ratio,
    float Time_mod, float Resolution_bias
)
{
    vec2 uv = in_uv;
    uv.x *= aspect_ratio;

    vec2 R = gl_FragCoord.xy / uv * Resolution_bias;
    vec2 U = (2. * _uv * R - R) / R.y; // normalized coordinates

    U += .5 * U * mat2(cos(2. * U + Time_mod), sin(2. * U - Time_mod)); // distorted coordinates
    U *= 4.;

    vec4 O = vec4(0.);

    mat2 J = transpose(mat2(dFdx(U), dFdy(U))) * R.y / 8.; // the Jacobian store the local distorted frame

    // mat2 J = mat2(dFdx(U), dFdy(U)) * R.y / 8.; // the Jacobian store the local distorted frame
    return mix(in_uv, _uv * J, effect_intensity);
}