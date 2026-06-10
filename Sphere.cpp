#include "Sphere.h"
#include <cmath>


Sphere::Sphere(Vec3 c, float r, Color col, float refl)
    : center(c), radius(r), color(col), reflectivity(refl) {
}

bool Sphere::intersect(const Ray& ray, Intersection& hit) const {
    Vec3 oc = ray.origin - center;

    float a = ray.direction.dot(ray.direction);
    float b = 2.0f * oc.dot(ray.direction);
    float c = oc.dot(oc) - radius * radius;

    float disc = b * b - 4 * a * c;

    if (disc < 0) return false;

    float t = (-b - sqrt(disc)) / (2 * a);

    if (t < 0) return false;

    hit.t = t;
    hit.point = ray.origin + ray.direction * t;
    hit.normal = (hit.point - center).normalize();
    hit.color = color;
    hit.reflectivity = reflectivity;

    return true;
}