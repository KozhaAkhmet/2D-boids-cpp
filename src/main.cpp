#include <SFML/Graphics.hpp>
#include <random>

#include "simulation.hpp"

using namespace SimMath;

int main() {
  sf::RenderWindow window(sf::VideoMode(window_size_x, window_size_y),
                          "Boids Algorothm");
  window.setFramerateLimit(max_framerate);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(0.0f, 1.0f);

  // --- Setup ---
  Simulation::Instance simulation{window_size_x, window_size_y};
  simulation.generate(gen, dis, num_of_fish, col_radius, speed, icon_size, dt);

  // --- Main Loop ---
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    window.clear();
    simulation.run(window);
    window.display();
  }
  return 0;
}
