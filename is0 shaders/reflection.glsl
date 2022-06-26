vec3 reflet_out    = reflect(rd, normal);
vec3 refraction_in = refract(rd, normal, 1. / IOR);

vec3        p_enter = p - normal * SURF_DIST * 3.;
RayMarchRes d_in    = rayMarching(p_enter, refraction_in, INVERT_SDF); // Inside
float       d2      = d_in.dist;

vec3  p_exit         = p_enter + refraction_in * d2;
vec3  norm_exit      = -getNormal(p_exit);
vec3  reflect_text   = vec3(0);
vec3  refraction_out = vec3(0);
float abb            = .01;
// Red
refraction_out = refract(refraction_in, norm_exit, IOR - abb);
if (dot(refraction_out, refraction_out) == 0.)
{
    refraction_out = reflect(refraction_in, norm_exit);
}
reflect_text.r = refraction_out.r;
// Green
refraction_out = refract(refraction_in, norm_exit, IOR);
if (dot(refraction_out, refraction_out) == 0.)
{
    refraction_out = reflect(refraction_in, norm_exit);
}
reflect_text.g = refraction_out.g;
// Blue
refraction_out = refract(refraction_in, norm_exit, IOR + abb);
if (dot(refraction_out, refraction_out) == 0.)
{
    refraction_out = reflect(refraction_in, norm_exit);
}
reflect_text.b = refraction_out.b;

float density            = .1;
float optical_distance   = exp(-d2 * density);
reflect_text             = reflect_text * optical_distance;
float fresnel_reflection = pow(1. + dot(rd, normal), 5.);

finalCol += mix(reflect_text, reflet_out, fresnel_reflection);