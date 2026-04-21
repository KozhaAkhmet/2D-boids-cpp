#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <string>
#include "consts.hpp"

//TODO Debug compile flags arent working
class Fish : public sf::CircleShape {
public:
     static int count;
     std::string name;
     Fish() {};
     Fish(float _col_radius, float _speed, float _size, float _dir, float _dt,
          float _pos_x, float _pos_y);
          
          void setCumulativeDirection(float rad) {
               if(rad != 0)
               this->dir += (abs(rad) / rad * this->turn_speed);
          this->dir = std::fmod(this->dir, SimMath::PI_M_2); 
          this->setRotation(sf::radians(this->dir));
     }
     void setDirection(float rad)
     {
          this->dir = rad;
          this->setRotation(sf::radians(this->dir));
     }
     void setCollisionRadius(float col) { this->col_radius = col; }
     void setSpeed(float speed) { this->speed = speed; }
     void setSepVec(sf::Vector2f _sep_vec){sep_vec = _sep_vec;};
     void setAlignAngle(float rad){allign_ang_rad = rad;};
     void setCohesionVec(sf::Vector2f vec){this->coh_vec = vec;};
     #ifndef NDEBUG  
          void drawCollisionDebug(sf::RenderWindow& window);
          void drawTrimmedCircle(float deg_value);
          void setDirLines(sf::VertexArray _sep_lines, sf::VertexArray _align_lines, sf::VertexArray _coh_lines){
               sep_lines = _sep_lines;
               align_lines = _align_lines;
               coh_lines = _coh_lines;
          };
          void setSepDirLines(sf::VertexArray _sep_lines){sep_lines = _sep_lines;};
          void setAlignDirLines(sf::VertexArray _align_lines){align_lines = _align_lines;};
          void setCohDirLines(sf::VertexArray _coh_lines){coh_lines = _coh_lines;};
          void setAffectLines(sf::VertexArray arr){affect_lines = arr;};
     #endif

     void setMinDistance(float dis){min_distance = dis;};

     float getDirection() const { return this->dir; }
     float getCollisionRadius() { return col_radius; }
     float getSpeed(){return speed;};
     float getMinDistance(){return min_distance;};
     float getAlignAngle(){return allign_ang_rad;};
     sf::Vector2f getSepVec(){return sep_vec;};
     sf::Vector2f getCohVec(){return coh_vec;};

     #ifndef NDEBUG
          sf::VertexArray collision_lines;
          sf::VertexArray affect_lines;
          sf::VertexArray sep_lines;
          sf::VertexArray align_lines;
          sf::VertexArray coh_lines;
     #endif

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

