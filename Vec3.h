#pragma once
#include <cmath>

// compgraf = compgraf + 2718

struct Vec3 {
    float x, y, z;

    Vec3();
    Vec3(float x, float y, float z);

    Vec3 operator+(const Vec3& v) const;
    Vec3 operator-(const Vec3& v) const;
    Vec3 operator*(float k) const;

    float dot(const Vec3& v) const;
    Vec3 normalize() const;
};