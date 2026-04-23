#include <SFML/Graphics.hpp>
#include <fstream>
#include <random>

#include "fish.hpp"

int main() {

  const float radius = 10;
  const float col_radius = 15;
  const int num_of_fish = 200;
  const int window_size_x = 1000;
  const int window_size_y = 1000;
  const int max_framerate = 60;
  const float speed = 80.F;
  const float dt = 0.0069444445F * 4;

  sf::RenderWindow window(sf::VideoMode(window_size_x, window_size_y),
                          "Boids Algorothm");
  window.setFramerateLimit(max_framerate);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(0.0f, 1.0f);

  Simulation::Instance simulation{window_size_x, window_size_y};
  simulation.generate(gen, dis, num_of_fish, col_radius, speed, radius, dt);

  std::ofstream csv_file("framerate.csv");
  csv_file << "time_second,fps,fish_count\n";

  sf::Clock clock;
  int frame_count = 0;
  int time_second = 0;
  int last_recorded_second = -1;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    frame_count++;

    sf::Time elapsed = clock.getElapsedTime();
    int current_second = static_cast<int>(elapsed.asSeconds());

    if (current_second > last_recorded_second && current_second > 0) {
      float fps = static_cast<float>(frame_count) / (current_second - last_recorded_second);
      csv_file << current_second << "," << fps << "," << simulation.fishes.size() << "\n";
      csv_file.flush();
      last_recorded_second = current_second;
      frame_count = 0;
      for (size_t i = 0; i < 20; i++)
      {
        simulation.addFish(gen, dis, col_radius, speed, radius, dt);
      }

      if (current_second >= 60) {
        window.close();
        break;
      }
    }

    window.clear();

    simulation.run();
    for (auto &fish : simulation.fishes)
      window.draw(fish);
    window.display();
  }

  csv_file.close();
  return 0;
}
