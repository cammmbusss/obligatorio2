
#include "Triangle.h"
#include <cmath>

Triangle::Triangle(Vec3 a, Vec3 b, Vec3 c, Color col,
    float refl,
    float refr,
    float iorVal) {
    v0 = a; v1 = b; v2 = c;
    color = col;
    reflectivity = refl;
    refractivity = refr;
    ior = iorVal;
}

bool Triangle::intersect(const Ray& ray, Intersection& hit) const {
    const float EPS = 1e-6f;

    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;

    Vec3 h = ray.direction.cross(edge2);
    float a = edge1.dot(h);

    if (std::fabs(a) < EPS)
        return false;

    float f = 1.0f / a;
    Vec3 s = ray.origin - v0;
    float u = f * s.dot(h);

    if (u < 0.0f || u > 1.0f)
        return false;

    Vec3 q = s.cross(edge1);
    float v = f * ray.direction.dot(q);

    if (v < 0.0f || u + v > 1.0f)
        return false;

    float t = f * edge2.dot(q);

    if (t > EPS) {
        hit.t = t;
        hit.point = ray.origin + ray.direction * t;

        Vec3 normal = edge2.cross(edge1).normalize();
        if (normal.dot(ray.direction) > 0.0f)
            normal = normal * -1.0f;

        hit.normal = normal;
        hit.color = color;
        hit.reflectivity = reflectivity;
        hit.refractivity = refractivity;
        hit.ior = ior;
        return true;
    }

    return false;
}
