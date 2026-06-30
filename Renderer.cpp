#include "Renderer.h"
#include <fstream>
#include <algorithm>

Renderer::Renderer(int w, int h) : width(w), height(h) {
    framebuffer.resize(width * height);
}

void Renderer::render(Scene& scene, Camera& camera) {
    float aspect = (float)width / (float)height;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            float nx = (2 * (x + 0.5f) / (float)width - 1) * aspect;
            float ny = (1 - 2 * (y + 0.5f) / (float)height);

            Ray ray = camera.generateRay(nx, ny);

            Color color = scene.trace(ray, 0);

            setPixel(x, y, color);
        }
    }
}

void Renderer::setPixel(int x, int y, const Color& color) {
    framebuffer[y * width + x] = color;
}

void Renderer::saveToPPM(const std::string& filename) {
    std::ofstream file(filename);

    file << "P3\n" << width << " " << height << "\n255\n";

    for (const auto& c : framebuffer) {
        int r = std::min(255, (int)(c.r * 255));
        int g = std::min(255, (int)(c.g * 255));
        int b = std::min(255, (int)(c.b * 255));

        file << r << " " << g << " " << b << "\n";
    }

    file.close();
}