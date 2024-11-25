#include "fish.hpp"

#include <filesystem>

#include "iostream"
#include "math.h"

constexpr double PI = 3.14159265358979323846;
constexpr double PI_M_2 = 3.14159265358979323846 * 2;
constexpr double PI_S_2 = 3.14159265358979323846 / 2;

int Fish::count = 0;
Fish::Fish(float _col_radius, float _speed, float _size, float _dir = 0,
           float _dt = 0.0069444445F, float _pos_x = 0, float _pos_y = 0) {
  this->col_radius = _col_radius;
  this->speed = _speed;
  this->dt = _dt;
  this->dir = _dir;
  this->setOrigin(_size, _size);
  this->setRadius(_size);
  this->setPosition(_pos_x, _pos_y);
  this->setDirection(_dir);
  Fish::count++;
  this->name = "fish_" + std::to_string(count);
}

void Fish::updatePosition() {
  sf::Vector2f temp = this->getPosition();

  temp.x += speed * dt * cos(this->getDirection());
  temp.y += speed * dt * sin(this->getDirection());

  this->setPosition(temp);
}

void Fish::separation(const std::vector<Fish>& fishes) {
  sf::Vector2f sum = {};
  std::vector<const Fish*> nearest = getCollisions(fishes);
  for (auto& n : nearest) {
    sum += this->getPosition() - n->getPosition();
  }
  this->sep_vec = sf::Vector2f(sum.x, sum.y);
}

void Fish::alignment(const std::vector<Fish>& fishes) {
  int count{0};
  double sum = 0;
  std::vector<const Fish*> nearest = getCollisions(fishes);
  for (auto& n : nearest) {
    sum += this->dir - (n->getDirection());
    count++;
  }
  sum = sum / count;
  this->allign_ang_rad = sum;
}

void Fish::cohesion(const std::vector<Fish>& fishes) {
  sf::Vector2f sum_vec{};
  int size = fishes.size() - 1;
  for (auto& fish : fishes) {
    if (fish.name != this->name) sum_vec += fish.getPosition();
  }
  sum_vec = sf::Vector2f(sum_vec.x / size, sum_vec.y / size);
  this->coh_vec = this->getPosition() - sum_vec;
}

void Fish::drawCollisionDebug(sf::RenderWindow& window) {
  drawTrimmedCircle(this->dir);
  window.draw(this->affect_lines);
  window.draw(this->collision_lines);
window.draw(this->dir_lines);
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

float Fish::getDirection() const { return this->dir; }

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

        float distance_to_target =
            Simulation::getDistance(this->getPosition(), target->getPosition());
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

  this->setRotation(this->dir * 180 / PI);
}

void Fish::applyModifiedDirection() {
  // double distance_divider = 10 * (log10(this->min_distance) +
  // this->col_radius);
  double distance_divider = this->min_distance;
  // double distance_divider = 1

  sf::VertexArray lines(sf::Lines, 2);
  sf::Vector2f sum_vec = this->sep_vec;
  if (sum_vec.x != 0 && sum_vec.y != 0) {
    const double sep_const = 0.008, allign_const = 0.03, coh_const = 0.01;
    // --- Separation ---
    double sep_ang, evade_ang, sep_rel_ang, positive_sep_ang, positive_sep_rel_ang;
    sep_ang = -(atan2(sum_vec.x, sum_vec.y));
    positive_sep_ang = sep_ang < 0 ? sep_ang + PI_M_2 : sep_ang;

    sep_rel_ang = positive_sep_ang - this->dir + PI_S_2;
    positive_sep_rel_ang = sep_rel_ang < 0 ? sep_rel_ang + PI_M_2 : sep_rel_ang;

    evade_ang = sep_ang + PI_S_2;
    if (positive_sep_rel_ang < PI) this->dir += sep_const * abs(sep_rel_ang);
    if (positive_sep_rel_ang > PI) this->dir -= sep_const * abs(sep_rel_ang);
    // --- Alignment ---
    if (this->allign_ang_rad > PI)
      this->dir += allign_const * this->allign_ang_rad;
    if (this->allign_ang_rad < PI)
      this->dir -= allign_const * this->allign_ang_rad;
    // --- Cohesion ---
    double coh_rad, positive_coh_ang, coh_rel_ang, positive_coh_rel_ang;
    coh_rad = -(atan2(this->coh_vec.x, this->coh_vec.y));
    positive_coh_ang = coh_rad < 0 ? coh_rad + PI_M_2 : coh_rad;

    coh_rel_ang = positive_coh_ang - this->dir + PI_S_2;
    positive_coh_rel_ang =
        coh_rel_ang < 0 ? coh_rel_ang + PI_M_2 : coh_rel_ang;
    if (positive_coh_rel_ang < PI) this->dir -= coh_const * abs(coh_rel_ang);
    if (positive_coh_rel_ang > PI) this->dir += coh_const * abs(coh_rel_ang);

    // -- Direction Renderer Debugger ---
    lines[0].position = this->getPosition();
    lines[1].position =
        Simulation::polarToCortesian(coh_rad + PI_S_2) + this->getPosition();
    lines[1].color = sf::Color::Red;
  }
if (this->dir < 0) this->dir += PI_M_2;
  // TODO I have to add an extra 45 degrees for some reason. Im not figured
  // out yet.
  if (PI_M_2 + PI_S_2 / 2 < this->dir) this->dir -= PI_M_2;

  this->dir_lines = lines;
  this->setRotation(this->dir * 180 / PI);
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
    fish.getCollisions(fishes);
    fish.separation(fishes);
    fish.alignment(fishes);
    fish.cohesion(fishes);
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
