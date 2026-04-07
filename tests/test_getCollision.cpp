#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>

#include "fish.hpp"
#include "sim_math.hpp"

int main() {
  const float col_radius = 100;
  const int num_of_fish = 200;
  const int window_size_x = 300;
  const int window_size_y = 300;
  const int max_framerate = 60;
  const float speed = 80.F;
  const float dt = 0.0069444445F * 4;

  
  sf::RenderWindow window(sf::VideoMode(window_size_x, window_size_y),
  "test_getCollision");
  window.setFramerateLimit(max_framerate);
  
  std::vector<sf::Texture> imgmap;
  for (const auto& file : std::filesystem::directory_iterator("../res/")) {
    sf::Texture t;
    t.loadFromFile(file.path());
    t.setSmooth(true);
    t.generateMipmap();
    imgmap.push_back(t);
  }

  Fish dummy_fish(col_radius, speed, SimMath::icon_size, SimMath::PI + SimMath::PI_D_2,
                             dt, window_size_x/2,
                             window_size_y/2);

  dummy_fish.setTexture(&imgmap[3]);

  Fish cursorFish(col_radius, speed, SimMath::icon_size, SimMath::PI + SimMath::PI_D_2,
                             dt, window_size_x/2 - 20,
                             window_size_y/2);
  
  auto dummy_ptr = std::make_shared<Fish>(dummy_fish);
  auto cursor_ptr = std::make_shared<Fish>(cursorFish);
  
  std::vector<std::shared_ptr<Fish>> test_fishes = {dummy_ptr, cursor_ptr};

  float increment_ang= 1;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    window.clear();

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    if(worldPos.x<0)
      worldPos.x = 0;
    if(worldPos.y<0)
      worldPos.y = 0;
    cursor_ptr->setPosition(worldPos);
    
    SimMath::getCollisions(dummy_ptr, test_fishes, dummy_ptr->getCollisionRadius());
    dummy_ptr->drawCollisionDebug(window);

    dummy_ptr->setDirection(increment_ang++);
    window.draw(*cursor_ptr);
    window.draw(*dummy_ptr);

    window.display();
  }
  return 0;
}
