#include "Scene.h"
#include <algorithm>
#include <cmath>
#include <iostream>

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

    return I * eta + n * (eta * cosi - sqrt(k));
}

float fresnel(const Vec3& I, const Vec3& N, float ior) {
    float cosi = std::clamp(I.dot(N), -1.0f, 1.0f);
    float etai = 1.0f;
    float etat = ior;

    if (cosi > 0) {
        std::swap(etai, etat);
    }

    float sint = etai / etat * sqrtf(std::max(0.0f, 1 - cosi * cosi));

    if (sint >= 1.0f) {
        return 1.0f;
    }

    float cost = sqrtf(std::max(0.0f, 1 - sint * sint));
    cosi = fabs(cosi);

    float Rs = ((etat * cosi) - (etai * cost)) /
        ((etat * cosi) + (etai * cost));

    float Rp = ((etai * cosi) - (etat * cost)) /
        ((etai * cosi) + (etat * cost));

    return (Rs * Rs + Rp * Rp) / 2.0f;
}

Color Scene::trace(const Ray& ray, int depth) {

    Color background(0.85f, 0.85f, 0.85f);

    if (depth > 10) {
        return background;
    }

    Intersection closestHit;
    closestHit.t = 1e30f;

    bool hitSomething = false;

    for (auto obj : objects) {
        Intersection tempHit;

        if (obj->intersect(ray, tempHit)) {
            if (tempHit.t > 0.001f && tempHit.t < closestHit.t) {
                closestHit = tempHit;
                hitSomething = true;
            }
        }
    }

    if (!hitSomething) {
        return background;
    }

    Color finalColor(0, 0, 0);

    for (auto& light : lights) {

        Vec3 lightVec = light.position - closestHit.point;
        float dist2 = lightVec.dot(lightVec);
        float lightDistance = sqrt(dist2);

        Vec3 lightDir = lightVec.normalize();

        Vec3 shadowOrigin = closestHit.point + closestHit.normal * 0.001f;

        Color shadowTint(1.0f, 1.0f, 1.0f);
        bool blocked = false;

        Vec3 currentOrigin = shadowOrigin;
        float remainingDistance = lightDistance;

        for (int bounce = 0; bounce < 8; bounce++) {

            Ray currentShadowRay(currentOrigin, lightDir);

            Intersection closestShadowHit;
            closestShadowHit.t = 1e30f;

            bool shadowHitSomething = false;

            for (auto obj : objects) {
                Intersection tempShadowHit;

                if (obj->intersect(currentShadowRay, tempShadowHit)) {
                    if (tempShadowHit.t > 0.001f &&
                        tempShadowHit.t < remainingDistance &&
                        tempShadowHit.t < closestShadowHit.t) {

                        closestShadowHit = tempShadowHit;
                        shadowHitSomething = true;
                    }
                }
            }

            if (!shadowHitSomething) {
                break;
            }

            if (closestShadowHit.refractivity > 0.0f) {

                float transmission = 0.5f;

                Color glassShadowTint(
                    transmission + (1.0f - transmission) * closestShadowHit.color.r,
                    transmission + (1.0f - transmission) * closestShadowHit.color.g,
                    transmission + (1.0f - transmission) * closestShadowHit.color.b
                );

                shadowTint = shadowTint * glassShadowTint;

                currentOrigin = closestShadowHit.point + lightDir * 0.01f;
                remainingDistance -= closestShadowHit.t;
            }
            else {
                blocked = true;
                break;
            }
        }

        if (!blocked) {

            float diff = std::max(0.0f, closestHit.normal.dot(lightDir));
            float attenuation = light.intensity / (1.0f + dist2);

            Color diffuse =
                closestHit.color *
                shadowTint *
                (attenuation * diff);

            Vec3 viewDir = (ray.direction * -1.0f).normalize();

            Vec3 reflectLightDir =
                (closestHit.normal *
                    (2.0f * closestHit.normal.dot(lightDir))
                    - lightDir).normalize();

            float spec = pow(
                std::max(0.0f, viewDir.dot(reflectLightDir)),
                64.0f
            );

            Color specular =
                Color(1.0f, 1.0f, 1.0f) *
                shadowTint *
                (attenuation * spec * 0.6f);

            finalColor = finalColor + diffuse + specular;
        }
    }

    finalColor = finalColor + closestHit.color * 0.18f;

    Color reflectColor(0, 0, 0);

    if (closestHit.reflectivity > 0.0f) {
        Vec3 reflectDir =
            ray.direction - closestHit.normal * 2.0f * ray.direction.dot(closestHit.normal);

        reflectDir = reflectDir.normalize();

        Vec3 reflectOrigin = closestHit.point + closestHit.normal * 0.001f;
        Ray reflectRay(reflectOrigin, reflectDir);

        reflectColor = trace(reflectRay, depth + 1);
    }

    Color refractColor(0, 0, 0);

    if (closestHit.refractivity > 0.0f) {
        Vec3 refractDir = refract(ray.direction, closestHit.normal, closestHit.ior);
        refractDir = refractDir.normalize();

        Vec3 refractOrigin = closestHit.point + refractDir * 0.001f;
        Ray refractRay(refractOrigin, refractDir);

        refractColor = trace(refractRay, depth + 1);
    }

    if (closestHit.refractivity > 0.0f) {

        float kr = 0.0f;

        if (closestHit.reflectivity > 0.0f) {
            kr = fresnel(ray.direction, closestHit.normal, closestHit.ior);
        }

        float kt = std::clamp(closestHit.refractivity, 0.0f, 1.0f);
        float local = 1.0f - kt;

        Color glassTint(
            0.85f + 0.15f * closestHit.color.r,
            0.85f + 0.15f * closestHit.color.g,
            0.85f + 0.15f * closestHit.color.b
        );

        float reflectionBoost = std::clamp(
            kr + closestHit.reflectivity,
            0.0f,
            1.0f
        );

        finalColor =
            finalColor * local +
            reflectColor * reflectionBoost +
            (refractColor * glassTint) * kt * (1.0f - reflectionBoost);
    }
    else if (closestHit.reflectivity > 0.0f) {

        float kr = std::clamp(closestHit.reflectivity, 0.0f, 1.0f);

        finalColor =
            finalColor * (1.0f - kr) +
            reflectColor * kr;
    }

    return finalColor;
}

Color Scene::traceReflectionMap(const Ray& ray) {
    Intersection closestHit;
    closestHit.t = 1e30f;

    bool hitSomething = false;

    for (auto obj : objects) {
        Intersection tempHit;

        if (obj->intersect(ray, tempHit)) {
            if (tempHit.t > 0.001f && tempHit.t < closestHit.t) {
                closestHit = tempHit;
                hitSomething = true;
            }
        }
    }

    if (!hitSomething) {
        return Color(0, 0, 0);
    }

    float k = std::clamp(closestHit.reflectivity, 0.0f, 1.0f);

    return Color(k, k, k);
}

Color Scene::traceTransmissionMap(const Ray& ray) {
    Intersection closestHit;
    closestHit.t = 1e30f;

    bool hitSomething = false;

    for (auto obj : objects) {
        Intersection tempHit;

        if (obj->intersect(ray, tempHit)) {
            if (tempHit.t > 0.001f && tempHit.t < closestHit.t) {
                closestHit = tempHit;
                hitSomething = true;
            }
        }
    }

    if (!hitSomething) {
        return Color(0, 0, 0);
    }

    float k = std::clamp(closestHit.refractivity, 0.0f, 1.0f);

    return Color(k, k, k);
}