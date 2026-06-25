#include "cilindro.h"
#include <cmath>
#include <algorithm>

Cylinder::Cylinder(Vec3 base, Vec3 top, float radius, Color color,
    float refl, float refr, float iorVal)
    : base(base), top(top), radius(radius), color(color) {
    this->reflectivity = refl;
    this->refractivity = refr;
    this->ior = iorVal;
}

bool Cylinder::intersectLateralSurface(const Ray& ray, float& t1, float& t2) const {
    Vec3 axis = (top - base).normalize();
    float height = (top - base).length();

    
    Vec3 oc = ray.origin - base;

  
    Vec3 dPerp = ray.direction - axis * ray.direction.dot(axis);
    Vec3 ocPerp = oc - axis * oc.dot(axis);

    float a = dPerp.dot(dPerp);
    float b = 2.0f * dPerp.dot(ocPerp);
    float c = ocPerp.dot(ocPerp) - radius * radius;

    if (std::fabs(a) < 1e-8f) return false;

    float disc = b * b - 4.0f * a * c;
    if (disc < 0.0f) return false;

    float sqrtDisc = std::sqrt(disc);
    t1 = (-b - sqrtDisc) / (2.0f * a);
    t2 = (-b + sqrtDisc) / (2.0f * a);
    if (t1 > t2) std::swap(t1, t2);

    (void)height;
    return true;
}

bool Cylinder::intersectCap(const Ray& ray, const Vec3& capPoint, const Vec3& capNormal,
    Intersection& hit) const {
    float denom = capNormal.dot(ray.direction);
    if (std::fabs(denom) < 1e-6f) return false;

    float t = (capPoint - ray.origin).dot(capNormal) / denom;
    if (t < 0.0f) return false;

    Vec3 p = ray.origin + ray.direction * t;
    Vec3 diff = p - capPoint;
    float distFromAxis2 = diff.dot(diff);
    if (distFromAxis2 > radius * radius) return false; // fuera del disco

    hit.t = t;
    hit.point = p;
    hit.normal = capNormal;
    hit.color = color;
    hit.reflectivity = reflectivity;
    hit.refractivity = refractivity;
    hit.ior = ior;
    return true;
}

bool Cylinder::intersect(const Ray& ray, Intersection& hit) const {
    Vec3 axis = (top - base).normalize();
    float height = (top - base).length();

    Intersection best;
    best.t = 1e30f;
    bool found = false;


    float t1, t2;
    if (intersectLateralSurface(ray, t1, t2)) {
        
        float candidates[2] = { t1, t2 };
        for (float t : candidates) {
            if (t < 0.001f) continue;

            Vec3 p = ray.origin + ray.direction * t;
            float h = (p - base).dot(axis); // posicion a lo largo del eje

            if (h >= 0.0f && h <= height && t < best.t) {
                best.t = t;
                best.point = p;

               
                Vec3 axisPoint = base + axis * h;
                Vec3 normal = (p - axisPoint).normalize();

              
                if (ray.direction.dot(normal) > 0.0f) normal = normal * -1.0f;

                best.normal = normal;
                best.color = color;
                best.reflectivity = reflectivity;
                best.refractivity = refractivity;
                best.ior = ior;
                found = true;
            }
        }
    }

    
    Intersection capHit;
    if (intersectCap(ray, base, axis * -1.0f, capHit)) {
        if (capHit.t > 0.001f && capHit.t < best.t) {
            best = capHit;
            found = true;
        }
    }

   
    if (intersectCap(ray, top, axis, capHit)) {
        if (capHit.t > 0.001f && capHit.t < best.t) {
            best = capHit;
            found = true;
        }
    }

    if (found) hit = best;
    return found;
}