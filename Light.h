#pragma once
#include "Vec3.h"
#include "Color.h"


struct Light {
    Vec3 position;
    float intensity;

    Light(Vec3 p, float i);
};