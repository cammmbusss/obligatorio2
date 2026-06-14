#include "Sphere.h"
#include <cmath>


Sphere::Sphere(Vec3 c, float r, Color col,
    float refl, float refr, float iorVal)
{
    center = c;
    radius = r;
    color = col;
    reflectivity = refl;
    refractivity = refr;
    ior = iorVal;
}

bool Sphere::intersect(const Ray& ray, Intersection& hit) const {
    Vec3 oc = ray.origin - center;

    float a = ray.direction.dot(ray.direction);
    float b = 2.0f * oc.dot(ray.direction);
    float c = oc.dot(oc) - radius * radius;

    float disc = b * b - 4 * a * c;

    if (disc < 0) return false;

    float sqrtDisc = sqrt(disc);

    float t1 = (-b - sqrtDisc) / (2 * a);
    float t2 = (-b + sqrtDisc) / (2 * a);

    float t = t1;

    // si t1 es inválido usamos t2
    if (t < 0) t = t2;
    if (t < 0) return false;

    hit.t = t;
    hit.point = ray.origin + ray.direction * t;

    // normal
    Vec3 normal = (hit.point - center).normalize();

    // detecto si el rayo está entrando o saliendo
    bool outside = ray.direction.dot(normal) < 0;

    if (!outside) {
        normal = normal * -1.0f; // invertimos la normal si estamos dentro
    }

    hit.normal = normal;
    hit.color = color;
    hit.reflectivity = reflectivity;
    hit.refractivity = refractivity;
    hit.ior = ior;

    return true;
}