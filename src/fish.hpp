#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>
#include <string>

class Fish : public sf::CircleShape {
public:
     static int count;
     std::string name;
     Fish() {};
     Fish(float _col_radius, float _speed, float _size, float _dir, float _dt,
          float _pos_x, float _pos_y);
     void drawCollisionDebug(sf::RenderWindow& window);
     void drawTrimmedCircle(float deg_value);

     void setCummilativeDirection(float rad) {
          this->dir += (abs(rad) / rad * this->turn_speed);
          this->setRotation(sf::radians(this->dir));
     }
     void setDirection(float rad){this->dir = rad;}
     void setCollisionRadius(float col) { this->col_radius = col; }
     void setSpeed(float speed) { this->speed = speed; }
     void setSepVec(sf::Vector2f _sep_vec){sep_vec = _sep_vec;};
     void setAllignAngle(float rad){allign_ang_rad = rad;};
     void setVertexLines(sf::VertexArray);
     void setDirLines(sf::VertexArray _sep_lines, sf::VertexArray _align_lines, sf::VertexArray _coh_lines){
          sep_lines = _sep_lines;
          align_lines = _align_lines;
          coh_lines = _coh_lines;
     };
     void setMinDistance(float dis){min_distance = dis;};
     void setAffectLines(sf::VertexArray arr){affect_lines = arr;};

     float getDirection() const { return this->dir; }
     float getCollisionRadius() { return col_radius; }
     float getSpeed(){return speed;};
     float getMinDistance(){return min_distance;};
     float getAllignAngle(){return allign_ang_rad;};
     sf::Vector2f getSepVec(){return sep_vec;};
     sf::Vector2f getCohVec(){return coh_vec;};

     sf::VertexArray collision_lines;
     sf::VertexArray affect_lines;
     sf::VertexArray sep_lines;
     sf::VertexArray align_lines;
     sf::VertexArray coh_lines;

     ~Fish() {}

private:
     float dt;
     sf::Texture texture;

     float col_radius;
     float min_distance = 10000;
     int col_size;
     float speed;
     float dir;
     float turn_speed = 0.02f;
     sf::Vector2f sep_vec;
     sf::Vector2f coh_vec;
     float allign_ang_rad;
};

