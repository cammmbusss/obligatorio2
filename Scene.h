#pragma once
#include <vector>
#include "Object.h"
#include "Light.h"


class Scene {
public:
    std::vector<Object*> objects;
    std::vector<Light> lights;

    Color trace(const Ray& ray, int depth);
    Color traceReflectionMap(const Ray& ray);
    Color traceTransmissionMap(const Ray& ray);
};