#include <fstream>
#include <iostream>

#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Renderer.h"
#include "Camera.h"
#include "Pyramid.h"
#include "Box.h"
#include <algorithm>
#include "tinyxml2.h"



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

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            Color c = data[y * width + x];

            float rr = std::clamp(c.r, 0.0f, 1.0f);
            float gg = std::clamp(c.g, 0.0f, 1.0f);
            float bb = std::clamp(c.b, 0.0f, 1.0f);

            unsigned char r = (unsigned char)(255 * rr);
            unsigned char g = (unsigned char)(255 * gg);
            unsigned char b = (unsigned char)(255 * bb);

            unsigned char color[] = { b, g, r };
            file.write((char*)color, 3);
        }
    }

    file.close();
}

int main() {
    using namespace tinyxml2;

    // 1. Cargar XML
    XMLDocument doc;
    doc.LoadFile("scene.xml");
    XMLElement* root = doc.FirstChildElement("scene");

    // 2. Resolución
    XMLElement* resElem = root->FirstChildElement("resolution");
    int width = resElem->IntAttribute("width");
    int height = resElem->IntAttribute("height");
    Color* framebuffer = new Color[width * height];

    Scene scene;

    // 3. Cámara
    XMLElement* camElem = root->FirstChildElement("camera")->FirstChildElement("position");
    float cx = camElem->FloatAttribute("x");
    float cy = camElem->FloatAttribute("y");
    float cz = camElem->FloatAttribute("z");
    Camera camera(Vec3(cx, cy, cz));




    /*
    // Piso
    scene.objects.push_back(new Plane(
        Vec3(0, -2, 0), Vec3(0, 1, 0),   // piso en y = -2
        Color(1.0f, 1.0f, 1.0f)
    ));

    scene.objects.push_back(new Plane(
        Vec3(0, 2, 0), Vec3(0, -1, 0),   // techo en y = 2
        Color(1.0f, 1.0f, 1.0f)
    ));
    scene.objects.push_back(new Plane(
        Vec3(-3, 0, 0), Vec3(1, 0, 0),   // pared izquierda (roja)
        Color(0.8f, 0.2f, 0.2f)
    ));

    scene.objects.push_back(new Plane(
        Vec3(3, 0, 0), Vec3(-1, 0, 0),   // pared derecha (verde)
        Color(0.2f, 0.8f, 0.2f)
    ));
    scene.objects.push_back(new Plane(
        Vec3(0, 0, -9), Vec3(0, 0, 1),  // pared del fondo
        Color(1.0f, 1.0f, 1.0f)
    ));
    scene.objects.push_back(new Plane(
        Vec3(0, 0, 2),      // pared frontal
        Vec3(0, 0, -1),
        Color(0.0f, 0.0f, 0.0f)
    ));

    Color colorMesa(0.45f, 0.30f, 0.15f);

    float alturaPata = 1.0f;
	float alturaMesa = 0.2f;

    // Tabla: ancho y plano, apoyado sobre las patas
    // y va de -1.3 a -1.0  (cerca del piso pero elevado)
    scene.objects.push_back(new Box(
        Vec3(-1.8f, -2.0f + alturaPata, -8.5f),
        Vec3(1.8f, -2.0f + alturaPata + alturaMesa, -4.5f),
        colorMesa
    ));


    // Pata delantera izquierda
    scene.objects.push_back(new Box(
        Vec3(-1.7f, -2.0f , -5.2f),
        Vec3(-1.4f, -2.0f + alturaPata , -4.9f),
        colorMesa
    ));

    // Pata delantera derecha
    scene.objects.push_back(new Box(
        Vec3(1.4f, -2.0f  , -5.2f),
        Vec3(1.7f, -2.0f + alturaPata , -4.9f),
        colorMesa
    ));

    // Pata trasera izquierda
    scene.objects.push_back(new Box(
        Vec3(-1.7f, -2.0f , -8.3f),
        Vec3(-1.4f, -2.0f + alturaPata , -8.0f),
        colorMesa
    ));

    // Pata trasera derecha
    scene.objects.push_back(new Box(
        Vec3(1.4f, -2.0f , -8.3f),
        Vec3(1.7f, -2.0f + alturaPata, -8.0f),
        colorMesa
    ));
    */
    // 5. Objetos
    XMLElement* objectsElem = root->FirstChildElement("objects");

    XMLElement* planeElem = objectsElem->FirstChildElement("plane");
    while (planeElem) {
        float px = planeElem->FloatAttribute("px");
        float py = planeElem->FloatAttribute("py");
        float pz = planeElem->FloatAttribute("pz");
        float nx = planeElem->FloatAttribute("nx");
        float ny = planeElem->FloatAttribute("ny");
        float nz = planeElem->FloatAttribute("nz");
        float r = planeElem->FloatAttribute("r");
        float g = planeElem->FloatAttribute("g");
        float b = planeElem->FloatAttribute("b");
        scene.objects.push_back(new Plane(Vec3(px, py, pz), Vec3(nx, ny, nz), Color(r, g, b)));
        planeElem = planeElem->NextSiblingElement("plane");
    }
    // Mesa
    XMLElement* tableElem = objectsElem->FirstChildElement("table");
    if (tableElem) {
        float alturaPata = tableElem->FloatAttribute("alturaPata");
        float alturaMesa = tableElem->FloatAttribute("alturaMesa");
        float r = tableElem->FloatAttribute("colorR");
        float g = tableElem->FloatAttribute("colorG");
        float b = tableElem->FloatAttribute("colorB");
        Color colorMesa(r, g, b);

        // tablero
        scene.objects.push_back(new Box(
            Vec3(-1.8f, -2.0f + alturaPata, -8.5f),
            Vec3(1.8f, -2.0f + alturaPata + alturaMesa, -4.5f),
            colorMesa
        ));
        // patas (ejemplo, podés agregar todas)
        scene.objects.push_back(new Box(
            Vec3(-1.7f, -2.0f, -5.2f),
            Vec3(-1.4f, -2.0f + alturaPata, -4.9f),
            colorMesa
        ));
    }

/*
    // =====================
    // OBJETOS SOBRE LA MESA
    // =====================

    // Pirámide azul atrás (grande)
    scene.objects.push_back(new Pyramid(
        Vec3(-0.8f, -0.8f, -8.2f),
        1.2f,
        Color(0.2f, 0.2f, 1.0f)
    ));

    // Vidrio adelante
    scene.objects.push_back(new Sphere(
        Vec3(-1.1f, -0.2f, -5.8f),
        0.6f,
        Color(1.0f, 0.0f, 0.0f),
        0.05f, 0.95f, 1.5f
    ));

    // Pirámide naranja centro
    scene.objects.push_back(new Pyramid(
        Vec3(0.2f, -0.8f, -6.8f),
        0.8f,
        Color(1.0f, 0.35f, 0.1f)
    ));

    // Esfera espejo derecha
    scene.objects.push_back(new Sphere(
        Vec3(1.2f, -0.3f, -6.0f),
        0.5f,
        Color(1.0f, 1.0f, 1.0f),
        0.95f, 0.0f, 1.0f
    ));
    */
        XMLElement* sphereElem = objectsElem->FirstChildElement("sphere");
        while (sphereElem) {
            float x = sphereElem->FloatAttribute("cx");
            float y = sphereElem->FloatAttribute("cy");
            float z = sphereElem->FloatAttribute("cz");
            float radius = sphereElem->FloatAttribute("radius");
            float r = sphereElem->FloatAttribute("r");
            float g = sphereElem->FloatAttribute("g");
            float b = sphereElem->FloatAttribute("b");
            float refl = sphereElem->FloatAttribute("reflection");
            float transp = sphereElem->FloatAttribute("transparency");
            float refr = sphereElem->FloatAttribute("refraction");

            scene.objects.push_back(new Sphere(Vec3(x, y, z), radius, Color(r, g, b), refl, transp, refr));

            sphereElem = sphereElem->NextSiblingElement("sphere");
        }
        XMLElement* pyrElem = objectsElem->FirstChildElement("pyramid");
        while (pyrElem) {
            float x = pyrElem->FloatAttribute("cx");
            float y = pyrElem->FloatAttribute("cy");
            float z = pyrElem->FloatAttribute("cz");
            float size = pyrElem->FloatAttribute("size");
            float r = pyrElem->FloatAttribute("r");
            float g = pyrElem->FloatAttribute("g");
            float b = pyrElem->FloatAttribute("b");
            scene.objects.push_back(new Pyramid(Vec3(x, y, z), size, Color(r, g, b)));
            pyrElem = pyrElem->NextSiblingElement("pyramid");
        }

    //LUCES
    // 4. Luces
    XMLElement* lightsElem = root->FirstChildElement("lights")->FirstChildElement("light");
    while (lightsElem) {
        float lx = lightsElem->FloatAttribute("x");
        float ly = lightsElem->FloatAttribute("y");
        float lz = lightsElem->FloatAttribute("z");
        float intensity = lightsElem->FloatAttribute("intensity");
        scene.lights.push_back(Light(Vec3(lx, ly, lz), intensity));
        lightsElem = lightsElem->NextSiblingElement("light");
    }
    XMLElement* boxElem = objectsElem->FirstChildElement("box");
while(boxElem){
 float minx=boxElem->FloatAttribute("minx");
 float miny=boxElem->FloatAttribute("miny");
 float minz=boxElem->FloatAttribute("minz");
 float maxx=boxElem->FloatAttribute("maxx");
 float maxy=boxElem->FloatAttribute("maxy");
 float maxz=boxElem->FloatAttribute("maxz");
 float r=boxElem->FloatAttribute("r");
 float g=boxElem->FloatAttribute("g");
 float b=boxElem->FloatAttribute("b");
 scene.objects.push_back(new Box(Vec3(minx,miny,minz), Vec3(maxx,maxy,maxz), Color(r,g,b)));
 boxElem=boxElem->NextSiblingElement("box");
}


   // scene.lights.push_back(Light(Vec3(0.0f, 1.0f, -5.8f), 3.5f));   // principal
   //scene.lights.push_back(Light(Vec3(-1.5f, 0.8f, -4.5f), 2.0f));  // relleno izquierda
   //scene.lights.push_back(Light(Vec3(1.5f, 0.8f, -4.5f), 2.0f));   // relleno derecha
   //scene.lights.push_back(Light(Vec3(0.0f, 1.0f, -8.7f), 0.4f)); 


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
    saveBMP("C:\\Users\\juanp\\Desktop\\output.bmp", width, height, framebuffer);

    std::cout << "Render terminado: output.bmp" << std::endl;

    delete[] framebuffer;

    return 0;
}