#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "fish.hpp"
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
