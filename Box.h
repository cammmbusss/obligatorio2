#pragma once
#include "Triangle.h"
#include "Object.h"
#include "Scene.h"
#include <vector>

class Box : public Object {
public:
    std::vector<Triangle*> faces;

    Box(Vec3 minP, Vec3 maxP, Color color,
        float refl = 0.0f, float refr = 0.0f, float iorVal = 1.0f)
    {
        float x0 = minP.x, y0 = minP.y, z0 = minP.z;
        float x1 = maxP.x, y1 = maxP.y, z1 = maxP.z;

        // 8 vértices
        Vec3 v000(x0, y0, z0), v100(x1, y0, z0);
        Vec3 v010(x0, y1, z0), v110(x1, y1, z0);
        Vec3 v001(x0, y0, z1), v101(x1, y0, z1);
        Vec3 v011(x0, y1, z1), v111(x1, y1, z1);

        auto add = [&](Vec3 a, Vec3 b, Vec3 c) {
            faces.push_back(new Triangle(a, b, c, color, refl, refr, iorVal));
            };

        // Superior (+Y) — normal apunta hacia arriba
        add(v010, v011, v111);
        add(v010, v111, v110);

        // Inferior (-Y) — normal apunta hacia abajo
        add(v000, v100, v101);
        add(v000, v101, v001);

        // Frontal (+Z) — normal apunta hacia la cámara
        add(v001, v101, v111);
        add(v001, v111, v011);

        // Trasera (-Z) — normal apunta hacia el fondo
        add(v100, v000, v010);
        add(v100, v010, v110);

        // Derecha (+X)
        add(v100, v111, v101);
        add(v100, v110, v111);

        // Izquierda (-X) — normal apunta a la izquierda
        add(v000, v001, v011);
        add(v000, v011, v010);
    }

    bool intersect(const Ray& ray, Intersection& hit) const override {
        bool hitAny = false;
        hit.t = 1e30f;
        Intersection temp;

        for (auto* f : faces) {
            if (f->intersect(ray, temp) && temp.t < hit.t) {
                hit = temp;
                hitAny = true;
            }
        }
        return hitAny;
    }

    ~Box() {
        for (auto* f : faces) delete f;
    }
};