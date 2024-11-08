#include <SFML/Graphics.hpp>
#include <random>

class Fish : public sf::CircleShape {

public:
  static int count;
  std::string name;
  Fish() {};
  Fish(float col_radius, float speed, float size, float dir, float dt,
       float pos_x, float pos_y);
  void updatePosition();
  void avoid(const std::vector<Fish> &fishes);
  void mimicDirection(const std::vector<Fish> &from);
  void centerOfDirections();
  void drawCollisionDebug(sf::RenderWindow &window);
  void drawTrimmedCircle(float deg_value);
  float getDistance(const sf::Vector2f &b);
  float getDirection();
  std::vector<const Fish *> getCollisions(const std::vector<Fish> &fishes);
  void setCollisionRadius(float col);
  void setSpeed(float speed);
  void setDirection(float rad);

  sf::VertexArray collision_lines;
  sf::VertexArray affect_lines;

  ~Fish() {}

private:
  float dt;
  sf::Vector2f pos;
  sf::Texture texture;
  std::vector<Fish> nearest_fishes;

  float col_radius;
  float speed;
  double dir;
  float turn_speed = 0.04f;
};

namespace Simulation {
class Instance {
public:
  Instance(int _window_size_x, int _window_size_y);
  void run();
  void display();
  void generate(std::mt19937 &gen, std::uniform_real_distribution<float> dis,
                int number_of_fish, int col_radius, float speed, float radius,
                float dt);

  ~Instance() {}
  int window_size_x;
  int window_size_y;
  std::vector<Fish> fishes;

private:
  std::vector<sf::Texture> imgmap;

  sf::Vector2f checkBoundries(sf::Vector2f f);
};

float getDistance(const sf::Vector2f &a, const sf::Vector2f &b);
} // namespace Simulation
