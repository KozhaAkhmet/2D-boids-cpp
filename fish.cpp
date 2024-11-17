#include "fish.hpp"

#include <filesystem>

#include "iostream"
#include "math.h"

constexpr double PI = 3.14159265358979323846;
constexpr double PI_M_2 = 3.14159265358979323846 * 2;
constexpr double PI_S_2 = 3.14159265358979323846 / 2;
// TODO Rearrenge the fish methods according to fish.hpp
int Fish::count = 0;
Fish::Fish(float col_radius, float speed, float size, float dir = 0,
           float dt = 0.0069444445F, float pos_x = 0, float pos_y = 0) {
  this->col_radius = col_radius;
  this->speed = speed;
  this->dt = dt;
  this->dir = dir;
  this->setOrigin(size, size);
  this->setRadius(size);
  this->setPosition(pos_x, pos_y);
  this->setDirection(dir);
  Fish::count++;
  this->name = "fish_" + std::to_string(count);
}

void Fish::updatePosition() {
  sf::Vector2f temp = this->getPosition();

  temp.x += speed * dt * cos(this->getDirection());
  temp.y += speed * dt * sin(this->getDirection());

  this->setPosition(temp);
}

void Fish::avoid(const std::vector<Fish>& fishes) {
  sf::Vector2f sum = {};
  std::vector<const Fish*> nearest = getCollisions(fishes);
  for (auto& n : nearest) {
    sum += this->getPosition() - n->getPosition();
  }
  this->avoid_vec = sf::Vector2f(sum.x, sum.y);
}

void Fish::mimicDirection(const std::vector<Fish>& from) {
  float sum{};
  int count{};

  for (auto& n : this->nearest_fishes) {
    sum += this->dir - n.getDirection();
  }
  this->mimic_ang_rad = sum;
  // std::cout << this->mimic_ang_rad << std::endl;
}
// TODO FIsh path render debugger (trails)

void Fish::centerOfDirections() {}

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
    // std::cout << cur_deg << std::endl;
    lines[i].position.x = sin(cur_deg + offset_deg) * col_radius + init_pos.x;
    lines[i].position.y = cos(cur_deg + offset_deg) * col_radius + init_pos.y;
    cur_deg = cur_deg + view_deg / resolution;
    // std::cout << sin(cur_deg) * col_radius + init_pos.x << "   "
    //           << cos(cur_deg) * col_radius + init_pos.y << std::endl;
  }
  lines[resolution + 2] = init_pos;
  this->collision_lines = lines;
}

float Fish::getDirection() { return this->dir; }

std::vector<const Fish*> Fish::getCollisions(const std::vector<Fish>& fishes) {
  // TODO Cant make this method void due to performance issues. This should be
  // updated later or with this way we have to call it every time not once.
  std::vector<const Fish*> nearest = {};
  sf::VertexArray lines(sf::Lines, fishes.size() * 2);
  for (int i = 0; i < fishes.size(); i++) {
    const Fish* target = &fishes[i];
    // if shorter from certain co_radius and is not this fish.
    if ((this != target) &&
        Simulation::getDistance(this->getPosition(), target->getPosition()) <
            col_radius) {
      // Get the vector from this to target
      sf::Vector2f sub_vec = this->getPosition() - target->getPosition();
      // Get the angle of that vector

      double rad = (this->dir + PI_S_2) - atan2(sub_vec.x, -sub_vec.y);

      double rel_angle = rad < 0 ? rad + PI_M_2 : rad;

      rel_angle = rel_angle > PI_M_2 ? rel_angle - PI_M_2 : rel_angle;

      // std::cout << rad * 180 / PI << "  " << rel_angle * 180 / PI <<
      // std::endl;

      if ((PI / 4 < rel_angle && rel_angle < 7 * PI / 4)) {
        lines[i * 2 + 1].position = this->getPosition();
        lines[i * 2 + 1].color = sf::Color::Black;
        lines[i * 2].position = target->getPosition();

        float distance_to_target = Simulation::getDistance(this->getPosition(),target->getPosition());
        this->min_distance = distance_to_target < this->min_distance
                                 ? distance_to_target
                                 : this->min_distance;

        nearest.emplace_back(target);
      }
    }
  }
  // std::cout << this->name << " " << nearest.size() << std::endl;
  this->affect_lines = lines;
  return nearest;
}

void Fish::setCollisionRadius(float col) { this->col_radius = col; }

void Fish::setSpeed(float speed) { this->speed = speed; }

void Fish::setDirection(float rad) {
  this->dir += (abs(rad) / rad * this->turn_speed);
  // std::cout << this->name << " " << this->dir << " " << distance_divider
  //           << std::endl;
  this->setRotation(this->dir * 180 / PI);
}

void Fish::applyModifiedDirection() {
  // TODO Does the conditions nessesary?
  // TODO simplyfy the atan2(sub.x,sub.y) to distribute throught method
  float rad;
  this->avoid_vec.y = -this->avoid_vec.y;
  // double distance_divider = 10 * (log10(this->min_distance) +
  // this->col_radius);
  double distance_divider = this->min_distance / 10;

  if (this->avoid_vec.y < 0) {
    // rad = -(atan(this->avoid_vec.x / this->avoid_vec.y) - PI / 2) /
    //           distance_divider +
    //       this->mimic_ang_rad * distance_divider;
    // (rad = this->mimic_ang_rad - PI / 2) * distance_divider;
    rad = -(atan(this->avoid_vec.x / this->avoid_vec.y) - PI / 2);

    this->dir += (abs(rad) / rad * this->turn_speed) / distance_divider;

    this->setRotation(this->dir * 180 / PI);
  } else if (this->avoid_vec.y > 0) {
    // rad = -(atan(this->avoid_vec.x / this->avoid_vec.y) + PI / 2) /
    //           distance_divider +
    //       this->mimic_ang_rad * distance_divider;
    // (rad = this->mimic_ang_rad + PI / 2) * distance_divider;
    rad = -(atan(this->avoid_vec.x / this->avoid_vec.y) + PI / 2);

    this->dir += (abs(rad) / rad * this->turn_speed) / distance_divider;

    this->setRotation(this->dir * 180 / PI);
  }
  std::cout << distance_divider << std::endl;
}

namespace Simulation {
Instance::Instance(int _window_size_x, int _window_size_y)
    : window_size_x(_window_size_x), window_size_y(_window_size_y) {
  for (const auto& file : std::filesystem::directory_iterator("res/")) {
    sf::Texture t;
    t.loadFromFile(file.path());
    t.setSmooth(true);
    t.generateMipmap();
    imgmap.push_back(t);
  }
}

void Instance::run() {
  for (Fish& fish : fishes) {
    fish.updatePosition();
    checkBoundries(fish);
    // fish.setPosition(checkBoundries(fish.getPosition()));

    fish.getCollisions(fishes);
    fish.avoid(fishes);
    fish.mimicDirection(fishes);
    //  centerOfDirections();
    fish.applyModifiedDirection();
  }
}

void Instance::display(sf::RenderWindow& _window) {
  for (auto& fish : fishes) {
    _window.draw(fish);
  }
}

// Generates the fiishes, also randomizes fishes position, direction and
// texture.
void Instance::generate(std::mt19937& gen,
                        std::uniform_real_distribution<float> dis,
                        int number_of_fish, int col_radius, float speed,
                        float radius, float dt) {
  for (int i = 0; i < number_of_fish; i++) {
    fishes.emplace_back(Fish(col_radius * 5, speed, radius, (dis(gen) * PI_M_2),
                             dt, (dis(gen) * window_size_x),
                             (dis(gen) * window_size_y)));
    // Choose randomly fishes from res file
    // fishes[i].setTexture(&imgmap[dis(gen) * imgmap.size()]);
    fishes[i].setTexture(&imgmap[3]);
  }
}

// Method checks the boundaries of fish. If its out of bounds then modifies its
// position.
void Instance::checkBoundries(Fish& fish) {
  sf::Vector2f temp = fish.getPosition();
  if (temp.x > window_size_x)
    temp.x = 0;
  else if (temp.x < 0)
    temp.x = window_size_x;
  if (temp.y > window_size_y)
    temp.y = 0;
  else if (temp.y < 0)
    temp.y = window_size_y;
  fish.setPosition(temp);
}

float getDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
  sf::Vector2f sub = a - b;
  return sqrt(sub.x * sub.x + sub.y * sub.y);
}

sf::Vector2f polarToCortesian(double rad) {
  return sf::Vector2f(cos(rad) * 50, sin(rad) * 50);
}

double cortesianToPolar(sf::Vector2f vec) { return atan(vec.x / vec.y); }

bool operator==(const Fish& lhs, const Fish& rhs) {
  return lhs == rhs ? true : false;
}

bool operator!=(const Fish& lhs, const Fish& rhs) {
  return lhs == rhs ? false : true;
}

}  // namespace Simulation
