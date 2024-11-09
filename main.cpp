#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#include "fish.hpp"

int main() {
  const float radius = 10;
  const float col_radius = 15;
  const int num_of_fish = 100;
  const int window_size_x = 1000;
  const int window_size_y = 1000;
  const int max_framerate = 30;
  const float speed = 80.F;
  const float dt = 0.0069444445F * 4;

  sf::RenderWindow window(sf::VideoMode(window_size_x, window_size_y),
                          "Boids Algorothm");
  window.setFramerateLimit(max_framerate);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(0.0f, 1.0f);

  // -- Creating Dummy Fish For Debug ---
  Fish dummyfish(col_radius * 5, speed, radius, M_PI / 2, dt, window_size_x / 2,
                 window_size_x / 2);
  dummyfish.setTextureInPlace("res/fish4.png");

  //-- Creating Fishes --
  std::vector<Fish> clown_fish;
  clown_fish.reserve(num_of_fish+1);

  for (int i = 0; i < num_of_fish; i++) {
    clown_fish.emplace_back(
        Fish(col_radius * 5, speed, radius, (dis(gen) * M_2_PI), dt,
             (dis(gen) * window_size_x), (dis(gen) * window_size_y)));
    clown_fish[i].setTextureInPlace("res/fish2.png");
  }
  // clown_fish.emplace_back(dummyfish);
  // --------------------

  // --- Setup ---
  Fish::setup(clown_fish, window_size_x, window_size_y);

  // --- Main Loop ---
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }
    window.clear();

    // --- Looping through all fishes ---
    for (int i = 0; i < num_of_fish; i++) {
      Fish::fishes[i].startEvent();
      // Fish::fishes[i].drawCollisionDebug(window);
    }
    // dummyfish.startEvent();

    // --- Rendering ---
    for (auto& fish : Fish::fishes) window.draw(fish);

    // dummyfish.drawCollisionDebug(window);
    // window.draw(dummyfish);
    window.display();
  }
  return 0;
}
