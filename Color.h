#pragma once

struct Color {
    float r, g, b;

    Color();
    Color(float r, float g, float b);

    Color operator+(const Color& c) const;
    Color operator*(float k) const;
};