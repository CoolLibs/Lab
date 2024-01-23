// TODO(JF) Move this to the Collision node
struct IntersectionResult2D {
    bool  has_intersected;
    vec2  intersection_point;
    float distance_along_ray;
};

struct IntersectionResult3D {
    bool  has_intersected;
    vec3  intersection_point;
    float distance_along_ray;
};