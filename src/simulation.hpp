#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <memory>
#include "fish.hpp"
class Simulation {
public:
    Simulation(unsigned int _window_size_x,unsigned int _window_size_y);
    void run(sf::RenderWindow& window);
    void display(sf::RenderWindow& _window);
    void generate(std::mt19937& gen, std::uniform_real_distribution<float> dis,
                    int number_of_fish, int col_radius, float speed, float radius,
                    float dt);
    void checkBoundries(Fish& fish);

    ~Simulation() {}
    int window_size_x;
    int window_size_y;

private:
    std::vector<sf::Texture> imgmap;
    std::vector<std::shared_ptr<Fish>> fishes;
};