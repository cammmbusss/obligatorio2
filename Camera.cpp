#include "Camera.h"

Camera::Camera(Vec3 pos) : position(pos) {}

Ray Camera::generateRay(float x, float y) {
    Vec3 dir = Vec3(x, y, -1).normalize();
    return Ray(position, dir);
}