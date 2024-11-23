#include <SFML/Graphics.hpp>
#include <random>

class Fish : public sf::CircleShape {
 public:
  static int count;
  std::string name;
  Fish() {};
  Fish(float _col_radius, float _speed, float _size, float _dir, float _dt,
       float _pos_x, float _pos_y);
  void updatePosition();
  void avoid(const std::vector<Fish>& fishes);
  void mimicDirection(const std::vector<Fish>& fishes);
  void centerOfFishes(std::vector<Fish>& fishes);
  void drawCollisionDebug(sf::RenderWindow& window);
  void drawTrimmedCircle(float deg_value);
  float getDirection() const;
  std::vector<const Fish*> getCollisions(const std::vector<Fish>& fishes);
  void setCollisionRadius(float col);
  void setSpeed(float speed);
  void setDirection(float rad);
  void applyModifiedDirection();

  sf::VertexArray collision_lines;
  sf::VertexArray affect_lines;
  sf::VertexArray dir_lines;

  ~Fish() {}

 private:
  float dt;
  sf::Vector2f pos;
  sf::Texture texture;

  float col_radius;
  float min_distance = 10000;
  int col_size;
  sf::Vector2f avoid_vec;
  double mimic_ang_rad;
  sf::Vector2f cof_vec;
  float speed;
  double dir;
  float turn_speed = 0.02f;
};

namespace Simulation {
class Instance {
 public:
  Instance(int _window_size_x, int _window_size_y);
  void run();
  void display(sf::RenderWindow& _window);
  void generate(std::mt19937& gen, std::uniform_real_distribution<float> dis,
                int number_of_fish, int col_radius, float speed, float radius,
                float dt);
  void checkBoundries(Fish& fish);

  ~Instance() {}
  int window_size_x;
  int window_size_y;
  std::vector<Fish> fishes;

 private:
  std::vector<sf::Texture> imgmap;
};

float getDistance(const sf::Vector2f& a, const sf::Vector2f& b);
sf::Vector2f polarToCortesian(double rad);
double cortesianToPolar(sf::Vector2f vec);
}  // namespace Simulation
