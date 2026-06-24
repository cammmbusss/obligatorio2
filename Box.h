#pragma once
#include "Triangle.h"
#include "Object.h"
#include "Scene.h"
#include <vector>

class Box : public Object {
public:
    std::vector<Triangle*> faces;

    Box(Vec3 minP, Vec3 maxP, Color color, float refl = 0.0f, float refr = 0.0f, float iorVal = 1.0f);

    bool intersect(const Ray& ray, Intersection& hit) const override;

    ~Box();
};