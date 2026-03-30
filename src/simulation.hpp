#pragma once

#include <SFML/Graphics.hpp>
#include <random>

class Fish;

namespace Simulation {
class Instance {
 public:
  Instance(int _window_size_x, int _window_size_y);
  void run(sf::RenderWindow& window);
  void display(sf::RenderWindow& _window);
  void generate(std::mt19937& gen, std::uniform_real_distribution<float> dis,
                int number_of_fish, int col_radius, float speed, float radius,
                float dt);
  void checkBoundries(Fish& fish);

  ~Instance() {}
  int window_size_x;
  int window_size_y;
  std::vector<Fish> fishes;

 private:
  std::vector<sf::Texture> imgmap;
};
}  // namespace Simulation
