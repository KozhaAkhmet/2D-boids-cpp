#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <memory>
#include "fish.hpp"
#include <iostream>

constexpr float icon_size = 10;
constexpr float col_radius = 100;
constexpr int num_of_fish = 200;
constexpr int window_size_x = 1000;
constexpr int window_size_y = 1000;
constexpr int max_framerate = 60;
constexpr float speed = 80.F;

constexpr float PI = 3.1415926F; //180 degree
constexpr float PI_M_2 = PI * 2; //360 degree
constexpr float PI_D_2 = PI / 2; //90 degree
constexpr float PI_D_4 = PI / 4; //45 degree

constexpr float dt = 0.0069444F * 4;

//TODO reverse parameters inside Simmath methods. Use fish to fishes relation 

class SimMath {
    public:
    SimMath(){};
    float getDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
        sf::Vector2f sub = a - b;
        return sqrt(sub.x * sub.x + sub.y * sub.y);
    }

    sf::Vector2f polarToCortesian(double rad) {
        return sf::Vector2f(cos(rad), sin(rad));
    }

    float cortesianToPolar(sf::Vector2f vec) {
        float ang_rad = atanf(vec.x / vec.y);
        if(vec.x == 0 || vec.y == 0)
            return 0;

        if(vec.x > 0 && vec.y > 0) //First Quadrant
            return ang_rad;
        else if(vec.x > 0 && vec.y < 0) //Fourth Quadrant
            return ang_rad + PI_M_2;
        else 
            return ang_rad + PI; //Second and Third are the same 
    }

    void updatePosition(std::shared_ptr<Fish> fish) {
        sf::Vector2f temp = fish->getPosition();

        temp.x += fish->getSpeed() * dt * cos(fish->getDirection());
        temp.y += fish->getSpeed() * dt * sin(fish->getDirection());

        fish->setPosition(temp);
    }

    void separation(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes) {
        sf::Vector2f sum = {};
        std::vector<const Fish*> nearest = getCollisions(fish, fishes, col_radius);
        for (auto& n : nearest) {
            sum += fish->getPosition() - n->getPosition();
        }
        fish->setSepVec(sum);
    }

    void alignment(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes) {
        int count{0};
        double sum = 0;
        std::vector<const Fish*> nearest = getCollisions(fish, fishes, col_radius * 2 / 3);
        for (auto& n : nearest) {
            sum += fish->getDirection() - (n->getDirection());
            count++;
        }
        sum = sum / count;
        fish->setAllignAngle(sum);
    }

    void cohesion(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes) {
        sf::Vector2f sum_vec{};
        std::vector<const Fish*> nearest = getCollisions(fish, fishes, col_radius * 1 / 3);
        int size = nearest.size();
        for (auto& fish : nearest) {
            sum_vec += fish->getPosition();
        }
        sum_vec = sf::Vector2f(sum_vec.x / size, sum_vec.y / size);
        if (size > 0) {
            fish->getCohVec() = fish->getPosition() - sum_vec;
        }
    }

    void applyModifiedDirection(std::shared_ptr<Fish> fish) {
        // double distance_divider = 10 * (log10(this->min_distance) +
        // this->col_radius);
        double distance_divider = fish->getMinDistance();
        // double distance_divider = 1

        sf::VertexArray lines(sf::Lines, 2);
        sf::Vector2f sum_vec = fish->getSepVec();
        sf::Vector2f coh_vec = fish->getCohVec();
        sf::Vector2f fish_pos = fish->getPosition();
        float dir = fish->getDirection();
        float allign_ang_rad = fish->getAllignAngle();

        if (sum_vec.x != 0 && sum_vec.y != 0) {
            const double sep_const = 0.004, allign_const = 0.007, coh_const = 0.006;
            // --- Separation ---
            double sep_ang, evade_ang, sep_rel_ang, positive_sep_ang,
                positive_sep_rel_ang;
            sep_ang = -(atan2(sum_vec.x, sum_vec.y));
            positive_sep_ang = sep_ang < 0 ? sep_ang + PI_M_2 : sep_ang;

            sep_rel_ang = positive_sep_ang - dir + PI_D_2;
            positive_sep_rel_ang = sep_rel_ang < 0 ? sep_rel_ang + PI_M_2 : sep_rel_ang;

            evade_ang = sep_ang + PI_D_2;
            if (positive_sep_rel_ang < PI) dir += sep_const * abs(sep_rel_ang);
            if (positive_sep_rel_ang > PI) dir -= sep_const * abs(sep_rel_ang);
            // --- Alignment ---
            if (allign_ang_rad > PI)
            dir += allign_const * allign_ang_rad;
            if (allign_ang_rad < PI)
            dir -= allign_const * allign_ang_rad;
            // --- Cohesion ---
            double coh_rad, positive_coh_ang, coh_rel_ang, positive_coh_rel_ang;
            coh_rad = -(atan2(coh_vec.x, coh_vec.y));
            positive_coh_ang = coh_rad < 0 ? coh_rad + PI_M_2 : coh_rad;

            coh_rel_ang = positive_coh_ang - dir + PI_D_2;
            positive_coh_rel_ang = coh_rel_ang < 0 ? coh_rel_ang + PI_M_2 : coh_rel_ang;
            if (positive_coh_rel_ang < PI) dir -= coh_const * abs(coh_rel_ang);
            if (positive_coh_rel_ang > PI) dir += coh_const * abs(coh_rel_ang);

            // -- Direction Renderer Debugger ---
            lines[0].position = fish_pos;
            lines[1].position =
                SimMath::polarToCortesian(sep_ang + PI_D_2) * fish.get()->getCollisionRadius() + fish_pos;
            lines[1].color = sf::Color::Red;
        }
        if (dir < 0) dir += PI_M_2;
        // TODO I have to add an extra 45 degrees for some reason. Im not figured
        // out yet.
        if (PI_M_2 + PI_D_2 / 2 < dir) dir -= PI_M_2;

        fish->setDirLines(lines);
        fish->setRotation(dir* 180 / PI);
    }

    std::vector<const Fish*> getCollisions(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes,
                                             int _col_radius) {
        // TODO Cant make this method void due to performance issues. This should be
        // updated later or with this way we have to call it every time not once.
        sf::Vector2f pos = fish->getPosition();
        std::vector<const Fish*> nearest = {};
        sf::VertexArray lines(sf::Lines, fishes.size() * 2);
        for (int i = 0; i < fishes.size(); i++) {
            const Fish* target = fishes.at(i).get();
            // if shorter from certain co_radius and is not this fish.
            if ((fish.get() != target) &&
                SimMath::getDistance(pos, target->getPosition()) < _col_radius) {
                // Get the vector from this to target
                sf::Vector2f sub_vec = pos - target->getPosition();
                // Get the angle of that vector

                double rad = (fmod(fish->getDirection(), PI_M_2) + PI_D_2) - atan2(sub_vec.x, -sub_vec.y);

                double rel_angle = rad < 0 ? rad + PI_M_2 : rad;

                rel_angle = rel_angle > PI_M_2 ? rel_angle - PI_M_2 : rel_angle;

                // std::cout << rad * 180 / PI << "  " << rel_angle * 180 / PI <<
                // std::endl;
                std::cout << fish->name << " " << nearest.size() <<  " distance " <<  SimMath::getDistance(pos, target->getPosition()) << std::endl;
                
                if ((PI / 4 < rel_angle && rel_angle < 7 * PI / 4)) {
                    lines[i * 2 + 1].position = pos;
                    lines[i * 2 + 1].color = sf::Color::Black;
                    lines[i * 2].position = target->getPosition();

                    float distance_to_target =
                        SimMath::getDistance(pos, target->getPosition());
                    fish->setMinDistance( distance_to_target < fish->getMinDistance()
                                            ? distance_to_target
                                            : fish->getMinDistance());

                    nearest.emplace_back(target);
                }
            }
        }
        fish->setAffectLines(lines);
        return nearest;
    }
};