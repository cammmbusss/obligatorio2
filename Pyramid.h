#pragma once

#include "Object.h"
#include "Triangle.h"
#include <vector>

class Pyramid : public Object {
public:
    std::vector<Triangle> faces;

    // Constructor
    Pyramid(Vec3 center, float size, Color color);

    // Intersección
    bool intersect(const Ray& ray, Intersection& hit) const override;
};