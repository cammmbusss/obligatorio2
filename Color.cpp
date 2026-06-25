#include "Color.h"

Color::Color() : r(0), g(0), b(0) {}

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}

Color Color::operator+(const Color& c) const {
    return Color(r + c.r, g + c.g, b + c.b);
}

Color Color::operator*(float k) const {
    return Color(r * k, g * k, b * k);
}

Color Color::operator*(const Color& c) const {
    return Color(r * c.r, g * c.g, b * c.b);
}