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
  this->dir = rad;
  this->setRotation(rad * 180 / PI);
}

float Fish::getDirection() { return this->dir; }

float Fish::getDistance(const sf::CircleShape& a, const sf::CircleShape& b) {
  sf::Vector2f sub = a.getPosition() - b.getPosition();
  return sub.x * sub.x + sub.y * sub.y;
}

float Fish::getDistance(const sf::Vector2f& b) {
  sf::Vector2f sub = this->getPosition() - b;
  return sub.x * sub.x + sub.y * sub.y;
}

void Fish::setTextureInPlace(std::string file_path) {
  this->texture.loadFromFile(file_path);
  this->texture.setSmooth(true);
  this->texture.generateMipmap();
  this->setTexture(&texture);
}

std::vector<Fish> Fish::getCollisions(const std::vector<Fish>& from) {
  std::vector<Fish> nearest;
  for (auto& sub : from) {
    if (this->getDistance(sub.getPosition()) < col_radius * col_radius) {
      nearest.push_back(sub);
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
  float rad = atan(sum.x / sum.y);
  // if (rad < 0) rad + PI / 2;
  sum.y = -sum.y;
  if (sum.x < 0 && sum.y < 0) {
    // third
    setDirection(-(rad) + PI / 2);

  } else if (sum.x < 0 && sum.y > 0) {
    // second
    setDirection(-(rad + PI / 2));

  } else if (sum.x > 0 && sum.y > 0) {
    // first
    setDirection(-rad - PI / 2);

  } else if (sum.x > 0 && sum.y < 0) {
    // fourth
    setDirection(-(rad - PI / 2));
  } else if (sum == sf::Vector2f(0, 0)) {
  }

  // std::cout << sum.x << " " << sum.y << " " << rad * 180 / PI << " " << count
  //           << std::endl;
}

Fish::~Fish() {}
