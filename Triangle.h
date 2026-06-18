#pragma once
#include "Object.h"

class Triangle : public Object {
public:
    Vec3 v0, v1, v2;
    Color color;

    Triangle(Vec3 a, Vec3 b, Vec3 c, Color col,
        float refl = 0.0f,
        float refr = 0.0f,
        float iorVal = 1.0f);

    bool intersect(const Ray& ray, Intersection& hit) const override;
};