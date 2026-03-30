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
  void separation(const std::vector<Fish>& fishes);
  void alignment(const std::vector<Fish>& fishes);
  void cohesion(const std::vector<Fish>& fishes);
  void drawCollisionDebug(sf::RenderWindow& window);
  void drawTrimmedCircle(float deg_value);
  float getDirection() const;
  std::vector<const Fish*> getCollisions(const std::vector<Fish>& fishes, int collision_range);
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
  float speed;
  double dir;
  float turn_speed = 0.02f;
  sf::Vector2f sep_vec;
  sf::Vector2f coh_vec;
  double allign_ang_rad;
};

