#include "fish.hpp"

#include "math.h"
#define PI 3.14159265358979323846

int Fish::count = 0;

Fish::Fish() { count++; }
Fish::Fish(float col_radius, float speed, float size, float dir = 0,
           float dt = 0.0069444445F, sf::Vector2f pos = sf::Vector2f(0, 0)) {
  this->col_radius = col_radius;
  this->speed = speed;
  this->dir = dir;
  this->size = size;
  this->setOrigin(size, size);
  this->setRadius(size);
  this->setPosition(pos);
  count++;
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

float Fish::getDirection() { return this->dir; }

float Fish::getDistance(const sf::CircleShape& a, const sf::CircleShape& b) {
  sf::Vector2f sub = a.getPosition() - b.getPosition();
  return sub.x * sub.x + sub.y * sub.y;
}

float Fish::getDistance(const sf::Vector2f& b) {
  sf::Vector2f sub = this->getPosition() - b;
  return sqrt(sub.x * sub.x + sub.y * sub.y);
}

void Fish::setTextureInPlace(std::string file_path) {
  this->texture.loadFromFile(file_path);
  this->texture.setSmooth(true);
  this->texture.generateMipmap();
  this->setTexture(&texture);
}

std::vector<Fish> Fish::getCollisions(const std::vector<Fish>& from) {
  std::vector<Fish> nearest;
  //TODO render a collision debugger 
  for (auto& s : from) {
    if (this->getDistance(s.getPosition()) < col_radius) {
      sf::Vector2f sub_vec = this->getPosition() - s.getPosition();
      float rad = -atan2(sub_vec.x, sub_vec.y);

      if (0 < (rad - this->dir) && (rad - this->dir) < PI) {
        std::cout << (rad - this->dir) * 180 / PI << std::endl;
        nearest.push_back(s);
      }
    }
  }
  return nearest;
}

void Fish::avoid(const std::vector<Fish>& from) {
  std::vector<Fish> nearest = getCollisions(from);
  sf::Vector2f sum = {};
  int count = 0;
  for (auto& n : nearest) {
    sum += getPosition() - n.getPosition();
    count++;
  }
  //TODO Does the conditions nessesary?
  //TODO simplyfy the atan2(sub.x,sub.y) to distribute throught methods
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
