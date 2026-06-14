#pragma once
#include "Vec3.h"
#include "Color.h"

struct Intersection {
    float t;
    Vec3 point;
    Vec3 normal;
    Color color;
    float reflectivity;
    float refractivity; 
    float ior;
};