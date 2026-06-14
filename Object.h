#pragma once
#include "Ray.h"
#include "Intersection.h"


class Object {
public:
    virtual bool intersect(const Ray& ray, Intersection& hit) const = 0;
    float reflectivity; 
    float refractivity;
    float ior;
};