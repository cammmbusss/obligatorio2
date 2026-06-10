#include "Scene.h"


Color Scene::trace(const Ray& ray, int depth) {

    if (depth > 3) return Color(0, 0, 0); // límite de rebotes

    Intersection closestHit;
    closestHit.t = 1e30;

    bool hitSomething = false;

    for (auto obj : objects) {
        if (obj->intersect(ray, closestHit)) {
            hitSomething = true;
        }
    }

    if (!hitSomething)
        return Color(0, 0, 0); // fondo

    Color finalColor(0, 0, 0);

    for (auto& light : lights) {

        Vec3 lightDir = (light.position - closestHit.point).normalize();

        float diff = closestHit.normal.dot(lightDir);
        if (diff < 0) diff = 0;

        finalColor = finalColor + closestHit.color * (light.intensity * diff);
    }

    float ambient = 0.1f;
    finalColor = finalColor + closestHit.color * ambient;

    if (closestHit.reflectivity > 0.0f) {

        Vec3 reflectDir = ray.direction -
            closestHit.normal * 2.0f * ray.direction.dot(closestHit.normal);

        reflectDir = reflectDir.normalize();

        Vec3 origin = closestHit.point + closestHit.normal * 0.001f;

        Ray reflectedRay(origin, reflectDir);

        Color reflectedColor = trace(reflectedRay, depth + 1);

        finalColor =
            finalColor * (1.0f - closestHit.reflectivity) +
            reflectedColor * closestHit.reflectivity;
    }

    return finalColor;
}