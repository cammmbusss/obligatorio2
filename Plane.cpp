#include "Plane.h"
#include <cmath>

Plane::Plane(Vec3 p, Vec3 n, Color c)
    : point(p), normal(n.normalize()), color(c) {
}

bool Plane::intersect(const Ray& ray, Intersection& hit) const {
    float denom = normal.dot(ray.direction);

    if (fabs(denom) < 1e-6) return false;

    float t = (point - ray.origin).dot(normal) / denom;

    if (t < 0) return false;

    hit.t = t;
    hit.point = ray.origin + ray.direction * t;
    hit.normal = normal;
    hit.color = color;

    return true;
}