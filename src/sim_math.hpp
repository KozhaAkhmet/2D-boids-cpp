#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

constexpr double PI = 3.14159265358979323846;
constexpr double PI_M_2 = 3.14159265358979323846 * 2;
constexpr double PI_S_2 = 3.14159265358979323846 / 2;

namespace SimMath {
    static float getDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
    sf::Vector2f sub = a - b;
    return sqrt(sub.x * sub.x + sub.y * sub.y);
    }

    static sf::Vector2f polarToCortesian(double rad) {
    return sf::Vector2f(cos(rad) * 50, sin(rad) * 50);
    }

    static double cortesianToPolar(sf::Vector2f vec) { return atan(vec.x / vec.y); }
}