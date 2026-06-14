#pragma once
#include "Object.h"


class Sphere : public Object {
public:
    Vec3 center;
    float radius;
    Color color;

    float reflectivity;   
    float refractivity;   
    float ior;            

    Sphere(Vec3 c, float r, Color col,
        float refl,
        float refr,
        float iorVal);

    bool intersect(const Ray& ray, Intersection& hit) const override;
};