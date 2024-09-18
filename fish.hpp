#include <SFML/Graphics.hpp>
#include <iostream>

class Fish : public sf::CircleShape {
 private:
  float size = 10;
  float dt;
  sf::Vector2f pos;
  sf::Texture texture;

  float col_radius;
  float speed;
  double dir;
  float turn_speed = 0.01f;

 public:
  static int count;
  Fish();
  Fish(float col_radius, float speed, float size, float dir, float dt,
       sf::Vector2f pos);
  float getDistance(const sf::CircleShape& a, const sf::CircleShape& b);
  float getDistance(const sf::Vector2f& b);
  void setTextureInPlace(std::string file_path);
  std::vector<Fish> getCollisions(const std::vector<Fish>& from);
  void avoid(const std::vector<Fish>& from);
  void setCollisionRadius(float col);
  void setSpeed(float speed);
  void setDirection(float rad);
  float getDirection();
  ~Fish();
};
