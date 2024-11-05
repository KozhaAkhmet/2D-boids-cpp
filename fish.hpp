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
  static std::vector<Fish> fishes;
  Fish();
  Fish( float col_radius, float speed, float size, float dir, float dt,
       float pos_x, float pos_y);
void setup(const std::vector<Fish>& from, int window_size_x, int window_size_y);
  void startEvent(const std::vector<Fish>& from);
  void updatePosition(int window_size_x, int window_size_y);
  void avoid(const std::vector<Fish>& from);
  void mimicDirection(const std::vector<Fish>& from);
  void centerOfDirections();
void drawCollisionDebug(sf::RenderWindow& window);
void drawTrimmedCircle(float deg_value);
  float getDistance(const sf::CircleShape& a, const sf::CircleShape& b);
  float getDistance(const sf::Vector2f& b);
  float getDirection();
  std::vector<Fish> getCollisions(const std::vector<Fish>& from);
  void setTextureInPlace(std::string file_path);
  void setCollisionRadius(float col);
  void setSpeed(float speed);
  void setDirection(float rad);

  sf::VertexArray collision_lines;
  sf::VertexArray affect_lines;


  ~Fish();
};
