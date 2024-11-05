#include "fish.hpp"

#include "math.h"
#define PI 3.14159265358979323846

int Fish::count = 0;

Fish::Fish() { count++; }
Fish::Fish(float col_radius, float speed, float size,
           float dir = 0, float dt = 0.0069444445F, float pos_x = 0,
           float pos_y = 0) {
  this->col_radius = col_radius;
  this->speed = speed;
  this->dt = dt;
  this->dir = dir;
  this->size = size;
  this->setOrigin(size, size);
  this->setRadius(size);
  this->setPosition(pos_x, pos_y);
  // this->setDirection(dir);
  count++;
}

float Fish::getDistance(const sf::CircleShape& a, const sf::CircleShape& b) {
  sf::Vector2f sub = a.getPosition() - b.getPosition();
  return sub.x * sub.x + sub.y * sub.y;
}

float Fish::getDistance(const sf::Vector2f& b) {
  sf::Vector2f sub = this->getPosition() - b;
  return sqrt(sub.x * sub.x + sub.y * sub.y);
}

float Fish::getDirection() { return this->dir; }

std::vector<Fish> Fish::getCollisions(const std::vector<Fish>& from) {
  std::vector<Fish> nearest;
  sf::VertexArray lines(sf::Lines, from.size() * 2);
  for (int i = 0; i < from.size(); i++) {
    // if shorter from certain co_radius
    if (this->getDistance(from[i].getPosition()) < col_radius) {
      // Get the vector from this to target
      sf::Vector2f sub_vec = this->getPosition() - from[i].getPosition();
      // Get the angle of that vector
      float rad = atan2(sub_vec.x, -sub_vec.y);

      double rel_angle = ((this->dir + PI / 2) - rad) < 0
                             ? ((this->dir + PI / 2) - rad) + 2 * PI
                             : ((this->dir + PI / 2) - rad);

      rel_angle = rel_angle > 2*PI ? rel_angle - PI*2 : rel_angle ;
      
      std::cout << ((this->dir + PI / 2) - rad) * 180 / PI << "  "
                << rel_angle * 180 / PI << " "
                << (this->dir - (rad + PI / 2)) * 180 / PI << std::endl;

      if ((PI / 4 < rel_angle && rel_angle < 7 * PI / 4)) {
        lines[i * 2 + 1].position = this->getPosition();
        lines[i * 2 + 1].color = sf::Color::Black;
        lines[i * 2].position = from[i].getPosition();
        
        nearest.push_back(from[i]);
      }
    }
  }
  this->affect_lines = lines;
  return nearest;
}

void Fish::setCollisionRadius(float col) { this->col_radius = col; }

void Fish::setSpeed(float speed) { this->speed = speed; }

void Fish::setDirection(float rad) {
  float sub = dir - rad;
  this->dir += abs(rad) / rad * this->turn_speed;
  this->setRotation(this->dir * 180 / PI);
  // std::cout << this->dir << std::endl;
}
// }

void Fish::setTextureInPlace(std::string file_path) {
  this->texture.loadFromFile(file_path);
  this->texture.setSmooth(true);
  this->texture.generateMipmap();
  this->setTexture(&texture);
}

void Fish::drawCollisionDebug(sf::RenderWindow& window) {
  drawTrimmedCircle(this->dir);
  window.draw(this->affect_lines);
  window.draw(this->collision_lines);
}

void Fish::drawTrimmedCircle(float deg_value) {
  int resolution = 10;
  sf::Vector2f init_pos = this->getPosition();
  float view_deg = 3 * PI / 2;
  float cur_deg = PI - PI / 8;
  // float offset_deg = cur_deg + view_deg / 2 + view_deg - deg_value;  // For
  // 45 degree and view_ang = PI/4 float offset_deg = cur_deg + view_deg / 2 -
  // deg_value;  // For 90 degree and view_ang = PI/2
  float offset_deg = cur_deg - deg_value;  // For 180 degree and view_ang = PI

  sf::VertexArray lines(sf::LineStrip, resolution + 3);
  lines[0].position = init_pos;

  for (int i = 1; i <= resolution + 1; i++) {
    std::cout << cur_deg << std::endl;
    lines[i].position.x = sin(cur_deg + offset_deg) * col_radius + init_pos.x;
    lines[i].position.y = cos(cur_deg + offset_deg) * col_radius + init_pos.y;
    cur_deg = cur_deg + view_deg / resolution;
    std::cout << sin(cur_deg) * col_radius + init_pos.x << "   "
              << cos(cur_deg) * col_radius + init_pos.y << std::endl;
  }
  lines[resolution + 2] = init_pos;
  this->collision_lines = lines;
}

void Fish::updatePosition(int window_size_x, int window_size_y) {
  sf::Vector2f temp = this->getPosition();
  if (temp.x > window_size_x)
    temp.x = 0;
  else if (temp.x < 0)
    temp.x = window_size_x;
  if (temp.y > window_size_y)
    temp.y = 0;
  else if (temp.y < 0)
    temp.y = window_size_y;

  temp.x += speed * dt * cos(this->getDirection());
  temp.y += speed * dt * sin(this->getDirection());
  this->setPosition(temp);
}

void Fish::avoid(const std::vector<Fish>& from) {
  std::vector<Fish> nearest = getCollisions(from);
  sf::Vector2f sum = {};
  int count = 0;
  for (auto& n : nearest) {
    sum += getPosition() - n.getPosition();
    count++;
  }
  // TODO Does the conditions nessesary?
  // TODO simplyfy the atan2(sub.x,sub.y) to distribute throught methods
  float rad, sub;
  sum.y = -sum.y;
  if (sum.y < 0) {
    // third
    rad = -(atan(sum.x / sum.y) - PI / 2);
    float sub = dir - rad;
    // if (-PI / 2 < sub && sub < PI / 2)
    setDirection(rad);

  } else if (sum.y > 0) {
    // second
    rad = -(atan(sum.x / sum.y) + PI / 2);
    float sub = dir - rad;
    // if (-PI / 2 < sub && sub < PI / 2)
    setDirection(rad);
  }
}

Fish::~Fish() {}
