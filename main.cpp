#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#include "fish.hpp"

int main() {
  const float radius = 10;
  const float col_radius = 10;
  const int num_of_fish = 10;
  const int window_size_x = 500;
  const int window_size_y = 500;
  const int max_framerate = 144;
  const float speed = 100.F;
  const float dt = 0.0069444445F;

  sf::RenderWindow window(sf::VideoMode(window_size_x, window_size_y),
                          "Boids Algorothm");
  window.setFramerateLimit(max_framerate);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(0.0f, 1.0f);

  // -- Creating Dummy Fish For Debug ---
  Fish dummyfish(col_radius * 5, speed, radius, 0, dt, window_size_x / 2,
                 window_size_x / 2);
  dummyfish.setTextureInPlace("res/fish4.png");
  
  //-- Creating Fishes --
  std::vector<Fish> clown_fish;
  clown_fish.reserve(num_of_fish);

  for (int i = 0; i < num_of_fish; i++) {
    clown_fish.emplace_back(
        Fish(col_radius * 5, speed, radius, (dis(gen) * M_2_PI), dt,
             (dis(gen) * window_size_x), (dis(gen) * window_size_y)));
    clown_fish[i].setTextureInPlace("res/fish2.png");
  }
  // --------------------

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    window.clear();
    // --- Main Loop ---
    for (int i = 0; i < num_of_fish; i++) {
      clown_fish[i].updatePosition(window_size_x, window_size_y);

      clown_fish[i].avoid(clown_fish);
      // clown_fish[i].drawCollisionDebug(window);
      // clown_fish[i].mimicDirection(clown_fish);
    }
    // --- Rendering ---
    for (auto& fish : clown_fish) window.draw(fish);
    
    dummyfish.drawCollisionDebug(window);
    dummyfish.avoid(clown_fish);
    window.draw(dummyfish);
    window.display();
  }
  return 0;
}
