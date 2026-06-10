#pragma once
#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include <vector>
#include <string>

class Renderer {
public:
    int width, height;

    Renderer(int w, int h);

    void render(Scene& scene, Camera& camera);

    void setPixel(int x, int y, const Color& color);

    void saveToPPM(const std::string& filename);

private:
    std::vector<Color> framebuffer;
};