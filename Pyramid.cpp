#include "Pyramid.h"

Pyramid::Pyramid(Vec3 center, float size, Color color) {
    float h = size;

    Vec3 p0 = center + Vec3(-h, 0, -h);
    Vec3 p1 = center + Vec3(h, 0, -h);
    Vec3 p2 = center + Vec3(h, 0, h);
    Vec3 p3 = center + Vec3(-h, 0, h);
    Vec3 top = center + Vec3(0, h, 0);

    // ✅ CORRECCIÓN: base dividida en dos triángulos con winding correcto
    // La base apunta hacia abajo (normal = 0,-1,0), orden CW visto desde arriba
    faces.emplace_back(p0, p2, p1, color, 0.2f, 0.0f, 1.0f);  // triángulo 1 base
    faces.emplace_back(p0, p3, p2, color, 0.2f, 0.0f, 1.0f);  // triángulo 2 base

    // ✅ CORRECCIÓN: caras laterales con winding CCW visto desde afuera
    faces.emplace_back(p1, p0, top, color, 0.2f, 0.0f, 1.0f);  // cara frontal (z negativo)
    faces.emplace_back(p2, p1, top, color, 0.2f, 0.0f, 1.0f);  // cara derecha
    faces.emplace_back(p3, p2, top, color, 0.2f, 0.0f, 1.0f);  // cara trasera
    faces.emplace_back(p0, p3, top, color, 0.2f, 0.0f, 1.0f);  // cara izquierda

    reflectivity = 0.2f;
    refractivity = 0.0f;
    ior = 1.0f;

    for (auto& f : faces) {
        f.reflectivity = reflectivity;
        f.refractivity = refractivity;
        f.ior = ior;
    }
}

bool Pyramid::intersect(const Ray& ray, Intersection& hit) const {

    bool hitSomething = false;
    Intersection temp;

    hit.t = 1e30f;

    for (const auto& f : faces) {
        if (f.intersect(ray, temp)) {
            if (temp.t < hit.t) {
                hit = temp;
                hitSomething = true;
            }
        }
    }

    return hitSomething;
}