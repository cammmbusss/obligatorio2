#include <fstream>
#include <iostream>

#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Renderer.h"
#include "Camera.h"
#include "Pyramid.h"
#include "Box.h"



// función para guardar BMP simple
void saveBMP(const char* filename, int width, int height, Color* data) {
    std::ofstream file(filename, std::ios::binary);

    int filesize = 54 + 3 * width * height;

    unsigned char fileHeader[14] = {
        'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0
    };

    unsigned char infoHeader[40] = {
        40,0,0,0,
        0,0,0,0,
        0,0,0,0,
        1,0,24,0
    };

    fileHeader[2] = (unsigned char)(filesize);
    fileHeader[3] = (unsigned char)(filesize >> 8);
    fileHeader[4] = (unsigned char)(filesize >> 16);
    fileHeader[5] = (unsigned char)(filesize >> 24);

    infoHeader[4] = (unsigned char)(width);
    infoHeader[5] = (unsigned char)(width >> 8);
    infoHeader[6] = (unsigned char)(width >> 16);
    infoHeader[7] = (unsigned char)(width >> 24);

    infoHeader[8] = (unsigned char)(height);
    infoHeader[9] = (unsigned char)(height >> 8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);

    file.write((char*)fileHeader, 14);
    file.write((char*)infoHeader, 40);

    for (int i = 0; i < width * height; i++) {
        unsigned char r = (unsigned char)(255 * data[i].r);
        unsigned char g = (unsigned char)(255 * data[i].g);
        unsigned char b = (unsigned char)(255 * data[i].b);

        unsigned char color[] = { b, g, r };
        file.write((char*)color, 3);
    }

    file.close();
}

int main() {
    int width = 800;
    int height = 600;


    Color* framebuffer = new Color[width * height];

    // escena
    Scene scene;


    // ==========================
    // CUARTO (CORNELL BOX SIMPLE)
    // ==========================

    // Piso
    scene.objects.push_back(
        new Plane(
            Vec3(0, -2, 0),     // punto
            Vec3(0, 1, 0),      // normal
            Color(0.8f, 0.8f, 0.8f)
        )
    );

    // Techo
    scene.objects.push_back(
        new Plane(
            Vec3(0, 2, 0),
            Vec3(0, -1, 0),
            Color(0.8f, 0.8f, 0.8f)
        )
    );

    // Pared izquierda (roja)
    scene.objects.push_back(
        new Plane(
            Vec3(-3, 0, 0),
            Vec3(1, 0, 0),
            Color(0.8f, 0.2f, 0.2f)
        )
    );

    // Pared derecha (verde)
    scene.objects.push_back(
        new Plane(
            Vec3(3, 0, 0),
            Vec3(-1, 0, 0),
            Color(0.2f, 0.8f, 0.2f)
        )
    );

    // Pared del fondo
    scene.objects.push_back(
        new Plane(
            Vec3(0, 0, -10),
            Vec3(0, 0, 1),
            Color(0.9f, 0.9f, 0.9f)
        )
    );

    Color colorMesa(0.55f, 0.55f, 0.55f);

    // Tablero: ancho y plano, apoyado sobre las patas
// y va de -1.3 a -1.0  (cerca del piso pero elevado)
    scene.objects.push_back(new Box(
        Vec3(-1.8f, -1.3f, -8.5f),   // min: x izq, Y ABAJO del tablero, z frente
        Vec3(1.8f, -1.0f, -4.5f),   // max: x der, Y ARRIBA del tablero, z atras
        colorMesa
    ));

    // Pata delantera izquierda
    scene.objects.push_back(new Box(
        Vec3(-1.6f, -2.0f, -5.0f),   // desde el piso (y=-2)
        Vec3(-1.2f, -1.3f, -4.6f),   // hasta abajo del tablero
        colorMesa
    ));

    // Pata delantera derecha
    scene.objects.push_back(new Box(
        Vec3(1.2f, -2.0f, -5.0f),
        Vec3(1.6f, -1.3f, -4.6f),
        colorMesa
    ));

    // Pata trasera izquierda
    scene.objects.push_back(new Box(
        Vec3(-1.6f, -2.0f, -8.4f),
        Vec3(-1.2f, -1.3f, -8.0f),
        colorMesa
    ));

    // Pata trasera derecha
    scene.objects.push_back(new Box(
        Vec3(1.2f, -2.0f, -8.4f),
        Vec3(1.6f, -1.3f, -8.0f),
        colorMesa
    ));

    // ==========================
    // ESFERAS
    // ==========================


    scene.objects.push_back(new Sphere(
        Vec3(-1.0f, -0.5f, -6.5f),  // centro subido para apoyarse en y=-1.5
        1.0f,
        Color(0.9f, 0.8f, 0.85f),
        0.3f, 0.7f, 1.5f
    ));

    scene.objects.push_back(new Sphere(
        Vec3(1.2f, -0.5f, -6.0f),
        0.6f,
        Color(1.0f, 1.0f, 1.0f),
        0.95f, 0.0f, 1.0f
    ));

    // Esfera izquierda (transparente/reflectiva)
 // centro en y = -1.0 + radio = -1.0 + 0.7 = -0.3
    scene.objects.push_back(new Sphere(
        Vec3(-0.8f, -0.3f, -6.5f),
        0.7f,
        Color(0.9f, 0.8f, 0.85f),
        0.3f, 0.0f, 1.5f
    ));

    // Esfera derecha (espejo)
    scene.objects.push_back(new Sphere(
        Vec3(0.9f, -0.4f, -6.0f),
        0.6f,
        Color(1.0f, 1.0f, 1.0f),
        0.95f, 0.0f, 1.0f
    ));

    scene.objects.push_back(
        new Pyramid(Vec3(0, 0, -6), 1.5f, Color(1, 0, 0))
    );

    // ==========================
    // LUCES
    // ==========================

    scene.lights.push_back(Light(Vec3(0.0f, 1.5f, -5.0f), 1.5f));
    scene.lights.push_back(Light(Vec3(-2.0f, 1.0f, -3.0f), 0.5f));

    
    Camera camera(Vec3(0, 0, 0));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            float nx = (2 * (x + 0.5f) / (float)width - 1);
            float ny = (1 - 2 * (y + 0.5f) / (float)height);

            Ray ray = camera.generateRay(nx, ny);
           
            Color color = scene.trace(ray, 0);

            framebuffer[y * width + x] = color;
        }
    }

    //guardo las imagenes
    saveBMP("C:\\Users\\Usuario\\Desktop\\output.bmp", width, height, framebuffer);

    std::cout << "Render terminado: output.bmp" << std::endl;

    delete[] framebuffer;

    return 0;
}