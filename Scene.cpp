#include "Scene.h"
#include <algorithm>

Vec3 refract(const Vec3& I, const Vec3& N, float ior) {
    float cosi = std::clamp(I.dot(N), -1.0f, 1.0f);
    float etai = 1.0f;
    float etat = ior;
    Vec3 n = N;

    if (cosi < 0) {
        cosi = -cosi;
    }
    else {
        std::swap(etai, etat);
        n = N * -1.0f;
    }

    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);

    if (k < 0) {
        return Vec3(0, 0, 0);
    }
    else {
        return I * eta + n * (eta * cosi - sqrt(k));
    }
}

float fresnel(const Vec3& I, const Vec3& N, float ior) {
    float cosi = std::clamp(I.dot(N), -1.0f, 1.0f);
    float etai = 1.0f;
    float etat = ior;

    if (cosi > 0) std::swap(etai, etat);

    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));

    if (sint >= 1) {
        return 1.0f; // reflexión total interna
    }
    else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabs(cosi);

        float Rs = ((etat * cosi) - (etai * cost)) /
            ((etat * cosi) + (etai * cost));

        float Rp = ((etai * cosi) - (etat * cost)) /
            ((etai * cosi) + (etat * cost));

        return (Rs * Rs + Rp * Rp) / 2.0f;
    }
}

Color Scene::trace(const Ray& ray, int depth) {

    if (depth > 4) return Color(0, 0, 0);

    Intersection closestHit;
    closestHit.t = 1e30;

    bool hitSomething = false;

    for (auto obj : objects) {
        if (obj->intersect(ray, closestHit)) {
            hitSomething = true;
        }
    }

    if (!hitSomething)
        return Color(0, 0, 0);

    Color finalColor(0, 0, 0);

    for (auto& light : lights) {

        Vec3 lightDir = (light.position - closestHit.point).normalize();

        float diff = closestHit.normal.dot(lightDir);
        if (diff < 0) diff = 0;

        finalColor = finalColor + closestHit.color * (light.intensity * diff);
    }

    float ambient = 0.1f;
    finalColor = finalColor + closestHit.color * ambient;

    Vec3 reflectDir = ray.direction -
        closestHit.normal * 2.0f * ray.direction.dot(closestHit.normal);

    reflectDir = reflectDir.normalize();

    Vec3 reflectOrigin = closestHit.point + closestHit.normal * 0.001f;
    Ray reflectRay(reflectOrigin, reflectDir);
    Color reflectColor = trace(reflectRay, depth + 1);

    Vec3 refractDir = refract(ray.direction, closestHit.normal, closestHit.ior);
    refractDir = refractDir.normalize();

    Vec3 refractOrigin = closestHit.point - closestHit.normal * 0.001f;
    Ray refractRay(refractOrigin, refractDir);
    Color refractColor = trace(refractRay, depth + 1);

    float kr = fresnel(ray.direction, closestHit.normal, closestHit.ior);

    if (closestHit.refractivity > 0.0f) {
        finalColor =
            reflectColor * kr +
            refractColor * (1.0f - kr);
    }
    else if (closestHit.reflectivity > 0.0f) {
        finalColor =
            finalColor * (1.0f - closestHit.reflectivity) +
            reflectColor * closestHit.reflectivity;
    }

    return finalColor;
}