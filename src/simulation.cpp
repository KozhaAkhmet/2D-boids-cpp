#include <filesystem>
#include <memory>

#include "simulation.hpp"
#include "fish.hpp"
#include "sim_math.hpp"

namespace Simulation {
Instance::Instance(int _window_size_x, int _window_size_y)
    : window_size_x(_window_size_x), window_size_y(_window_size_y) {
  for (const auto& file : std::filesystem::directory_iterator("res/")) {
    sf::Texture t;
    t.loadFromFile(file.path());
    t.setSmooth(true);
    t.generateMipmap();
    imgmap.push_back(t);
  }
}

void Instance::run(sf::RenderWindow& window){
  for (auto& fish : fishes) {
    SimMath::updatePosition(fish);
    checkBoundries(*fish);
    SimMath::separation(fish, fishes);
    SimMath::alignment(fish, fishes);
    SimMath::cohesion(fish, fishes);
    SimMath::applyModifiedDirection(fish);
    window.draw(*fish);
  }
}

void Instance::display(sf::RenderWindow& _window) {
  for (auto& fish : fishes) {
    _window.draw(*fish);
  }
}

void Instance::generate(std::mt19937& gen,
                        std::uniform_real_distribution<float> dis,
                        int number_of_fish, int col_radius, float speed,
                        float radius, float dt) {
  for (int i = 0; i < number_of_fish; i++) {
    fishes.emplace_back(std::make_shared<Fish>(col_radius * 5, speed, radius, (dis(gen) * PI_M_2),
                             dt, (dis(gen) * window_size_x),
                             (dis(gen) * window_size_y)));
    fishes[i]->setTexture(&imgmap[3]);
  }
}

void Instance::checkBoundries(Fish& fish) {
  sf::Vector2f temp = fish.getPosition();
  if (temp.x > window_size_x)
    temp.x = 0;
  else if (temp.x < 0)
    temp.x = window_size_x;
  if (temp.y > window_size_y)
    temp.y = 0;
  else if (temp.y < 0)
    temp.y = window_size_y;
  fish.setPosition(temp);
}

}  // namespace Simulation
