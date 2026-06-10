#pragma once
#include "Vec3.h"

struct Ray {
    Vec3 origin;
    Vec3 direction;

    Ray(const Vec3& o, const Vec3& d);
};