#pragma once
#include "Object.h"


class Plane : public Object {
public:
    Vec3 point;
    Vec3 normal;
    Color color;

    Plane(Vec3 p, Vec3 n, Color c);

    bool intersect(const Ray& ray, Intersection& hit) const override;
};