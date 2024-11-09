#include <SFML/Graphics.hpp>
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

  // --- Setup ---
  Simulation::Instance simulation{window_size_x, window_size_y};
  simulation.generate(gen, dis, num_of_fish, col_radius, speed, radius, dt);

  // --- Main Loop ---
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    window.clear();

    simulation.run();
    for (auto &fish : simulation.fishes)
      window.draw(fish);
    // --- Rendering ---
    window.display();
  }
  return 0;
}
