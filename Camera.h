#pragma once
#include "Ray.h"

class Camera {
public:
    Vec3 position;

    Camera(Vec3 pos);

    Ray generateRay(float x, float y);
};