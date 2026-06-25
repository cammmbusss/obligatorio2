
#pragma once
#include "Vec3.h"
#include "Color.h"
#include "Ray.h"
#include "Intersection.h"
#include "Object.h"

class Cylinder : public Object {
public:
    Vec3 base;     
    Vec3 top;      
    float radius;
    Color color;

    Cylinder(Vec3 base, Vec3 top, float radius, Color color,
        float refl = 0.0f, float refr = 0.0f, float iorVal = 1.0f);

    bool intersect(const Ray& ray, Intersection& hit) const override;

private:
    bool intersectLateralSurface(const Ray& ray, float& t1, float& t2) const;

    bool intersectCap(const Ray& ray, const Vec3& capPoint, const Vec3& capNormal,
        Intersection& hit) const;
};
