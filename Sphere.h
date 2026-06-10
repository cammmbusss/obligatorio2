#pragma once
#include "Object.h"


class Sphere : public Object {
public:
    Vec3 center;
    float radius;
    Color color;
    float reflectivity;

    Sphere(Vec3 c, float r, Color col, float refl);

    bool intersect(const Ray& ray, Intersection& hit) const override;
};