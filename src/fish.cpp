#include <math.h>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/System/Vector2.hpp>
#include "fish.hpp"
#include "consts.hpp"

int Fish::count = 0;
Fish::Fish(float _col_radius, float _speed, float _size, float _dir = 0,
		   float _dt = 0.0069444445F, float _pos_x = 0, float _pos_y = 0) {
    this->col_radius = _col_radius;
    this->speed = _speed;
    this->dt = _dt;
    this->dir = _dir;
    this->setOrigin(sf::Vector2f(_size,_size));
    this->setRadius(_size);
    this->setPosition(sf::Vector2f(_pos_x, _pos_y));
    this->setCumulativeDirection(_dir);
    Fish::count++;
    this->name = "fish_" + std::to_string(count);
}
#ifndef NDEBUG
void Fish::drawCollisionDebug(sf::RenderWindow& window) {
    drawTrimmedCircle(this->dir);
    window.draw(this->affect_lines);
    window.draw(this->collision_lines);
    window.draw(this->sep_lines);
    window.draw(this->align_lines);
    window.draw(this->coh_lines);

}

void Fish::drawTrimmedCircle(float deg_value) {
    int resolution = 10;
    sf::Vector2f init_pos = this->getPosition();
    float view_deg = 3 * SimMath::PI / 2;
    float cur_deg = SimMath::PI - SimMath::PI / 8;
    // float offset_deg = cur_deg + view_deg / 2 + view_deg - deg_value;  // For
    // 45 degree and view_ang = PI/4 float offset_deg = cur_deg + view_deg / 2 -
    // deg_value;  // For 90 degree and view_ang = PI/2
    float offset_deg = cur_deg - deg_value;  // For 180 degree and view_ang = PI

    sf::VertexArray lines(sf::PrimitiveType::LineStrip, resolution + 3);
    lines[0].position = init_pos;

    for (int i = 1; i <= resolution + 1; i++) {
        // std::cout << cur_deg << std::endl;
        lines[i].position.x = sin(cur_deg + offset_deg) * col_radius + init_pos.x;
        lines[i].position.y = cos(cur_deg + offset_deg) * col_radius + init_pos.y;
        cur_deg = cur_deg + view_deg / resolution;
        // std::cout << sin(cur_deg) * col_radius + init_pos.x << "   "
        //           << cos(cur_deg) * col_radius + init_pos.y << std::endl;
    }
    lines[resolution + 2].position = init_pos;
    this->collision_lines = lines;
}
#endif

