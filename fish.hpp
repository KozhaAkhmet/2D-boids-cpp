#include <SFML/Graphics.hpp>
#include <iostream>

class Fish : public sf::CircleShape {
 private:
  float size = 10;
  float dt;
  sf::Vector2f pos;
  sf::Texture texture;
  std::vector<Fish> nearest_fishes;

  float col_radius;
  float speed;
  double dir;
  float turn_speed = 0.01f;

  static int window_size_x;
  static int window_size_y;
  static std::vector<Fish> fishes;

 public:
  Fish();
  Fish(float col_radius, float speed, float size, float dir, float dt,
       float pos_x, float pos_y);
  static void setup(const std::vector<Fish>& from, int window_size_x,
             int window_size_y);
  void startEvent();
  void updatePosition();
  void avoid();
  void mimicDirection(const std::vector<Fish>& from);
  void centerOfDirections();
  void drawCollisionDebug(sf::RenderWindow& window);
  void drawTrimmedCircle(float deg_value);
  float getDistance(const sf::CircleShape& a, const sf::CircleShape& b);
  float getDistance(const sf::Vector2f& b);
  float getDirection();
  void getCollisions();
  void setTextureInPlace(std::string file_path);
  void setCollisionRadius(float col);
  void setSpeed(float speed);
  void setDirection(float rad);

  sf::VertexArray collision_lines;
  sf::VertexArray affect_lines;

  ~Fish();
};
