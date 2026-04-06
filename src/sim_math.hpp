#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "fish.hpp"

constexpr float icon_size = 10;
constexpr float col_radius = 100;
constexpr int num_of_fish = 200;
constexpr int window_size_x = 1000;
constexpr int window_size_y = 1000;
constexpr int max_framerate = 60;
constexpr float speed = 80.F;

constexpr float PI = 3.1415926F;
constexpr float PI_M_2 = PI * 2;
constexpr float PI_D_2 = PI / 2;
constexpr float PI_D_4 = PI / 4;

constexpr float dt = 0.0069444F * 4;

namespace SimMath {
    float getDistance(const sf::Vector2f& a, const sf::Vector2f& b);
    sf::Vector2f polarToCortesian(double rad);
    double cortesianToPolar(sf::Vector2f vec);
    void updatePosition(std::shared_ptr<Fish> fish);
    void separation(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes);
    void alignment(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes);
    void cohesion(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes);
    void applyModifiedDirection(std::shared_ptr<Fish> fish);
    std::vector<const Fish*> getCollisions(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes,
                                             int _col_radius);
};
