#include <SFML/System/Angle.hpp>
#include <iostream>
#include "sim_math.hpp"
#include "math.h"
#include "consts.hpp"

float SimMath::getDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
    sf::Vector2f sub = a - b;
    return sqrtf(sub.x * sub.x + sub.y * sub.y);
}

sf::Vector2f SimMath::polarToCortesian(float rad) {
    return sf::Vector2f(cosf(rad), sinf(rad));
}

float SimMath::cortesianToPolar(sf::Vector2f vec) {
    if(vec.x == 0 || vec.y == 0)
        return 0;

    float ang_rad = atanf(vec.x / vec.y);

    if(vec.x > 0 && vec.y > 0) //First Quadrant
        return ang_rad;
    else if(vec.x > 0 && vec.y < 0) //Fourth Quadrant
        return ang_rad + PI_M_2;
    else 
        return ang_rad + PI; //Second and Third are the same 
}

void SimMath::updatePosition(std::shared_ptr<Fish> fish) {
    sf::Vector2f temp = fish->getPosition();

    temp.x += fish->getSpeed() * dt * cos(fish->getDirection());
    temp.y += fish->getSpeed() * dt * sin(fish->getDirection());

    fish->setPosition(temp);
}

void SimMath::separation(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes_nearby) {
    sf::Vector2f sum = {};
    for (auto& n : fishes_nearby) {
        sum += fish->getPosition() - n->getPosition();
    }
    std::cout << sum.x << " " << sum.y << std::endl;
    fish->setSepVec(sum);
}

void SimMath::alignment(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes_nearby) {
    int count{0};
    double sum = 0;
    for (auto& n : fishes_nearby) {
        sum += fish->getDirection() - (n->getDirection());
        count++;
    }
    sum = sum / count;
    fish->setAllignAngle(sum);
}

void SimMath::cohesion(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes_nearby) {
    sf::Vector2f sum_vec{};
    int size = fishes_nearby.size();
    for (auto& fish : fishes_nearby) {
        sum_vec += fish->getPosition();
    }
    sum_vec = sf::Vector2f(sum_vec.x / size, sum_vec.y / size);
    if (size > 0) {
        fish->getCohVec() = fish->getPosition() - sum_vec;
    }
}

void SimMath::applyModifiedDirection(std::shared_ptr<Fish> fish) {
    float distance_divider = fish->getMinDistance();

    sf::Vector2f fish_pos = fish->getPosition();
    float dir = fish->getDirection();
    
    const float sep_const = 0.004, allign_const = 0.007, coh_const = 0.006;
    //------------Seperation----------
    sf::Vector2f sep_vec = fish->getSepVec();
    float sep_local_ang;
    if (sep_vec.x != 0 && sep_vec.y != 0) {
        float evade_ang, sep_local_dir_ang, sep_ang_relative_neg_y, positive_sep_local_ang;

        //The sep_vec is the sum of fish directions.
        //For fish, 0 angle is neg y and sep_ang_relative_neg_y become reletive to negative y.
        sep_ang_relative_neg_y = atan2f(sep_vec.x,sep_vec.y);
        
        //the sep_ang_relative_neg_y and dir are already directional (they can be negative).
        sep_local_ang = dir - sep_ang_relative_neg_y;

        sep_local_ang += PI; // The ang should be at opposite side

        dir += sep_const * sep_local_ang;
    }

    sf::VertexArray sep_lines(sf::PrimitiveType::LineStrip, 2);
    sep_lines[0].position = fish_pos;
    sep_lines[1].position =
        SimMath::polarToCortesian(sep_local_ang) * fish.get()->getCollisionRadius() + fish_pos;
    sep_lines[1].color = sf::Color::Red;

    //------------Alignment----------
    float allign_ang_rad = fish->getAllignAngle();
    if (allign_ang_rad > PI)
        dir += allign_const * allign_ang_rad;
    if (allign_ang_rad < PI)
        dir -= allign_const * allign_ang_rad;

    sf::VertexArray align_lines(sf::PrimitiveType::LineStrip, 2);
    align_lines[0].position = fish_pos;
    align_lines[1].position =
        SimMath::polarToCortesian(allign_ang_rad + PI_D_2) * fish.get()->getCollisionRadius() + fish_pos;
    align_lines[1].color = sf::Color::Green;

    //------------Cohesion----------
    sf::Vector2f coh_vec = fish->getCohVec();
    float coh_rad, positive_coh_ang, coh_rel_ang, positive_coh_rel_ang;
    if(coh_vec.x != 0 && coh_vec.y !=0){
        coh_rad = -(atan2(coh_vec.x, coh_vec.y));
        positive_coh_ang = coh_rad < 0 ? coh_rad + PI_M_2 : coh_rad;

        coh_rel_ang = positive_coh_ang - dir + PI_D_2;
        positive_coh_rel_ang = coh_rel_ang < 0 ? coh_rel_ang + PI_M_2 : coh_rel_ang;
        if (positive_coh_rel_ang < PI) dir -= coh_const * abs(coh_rel_ang);
        if (positive_coh_rel_ang > PI) dir += coh_const * abs(coh_rel_ang);
    }
    sf::VertexArray coh_lines(sf::PrimitiveType::LineStrip, 2);
    coh_lines[0].position = fish_pos;
    coh_lines[1].position =
        SimMath::polarToCortesian(coh_rad + PI_D_2) * fish.get()->getCollisionRadius() + fish_pos;
    coh_lines[1].color = sf::Color::Yellow;
    
    //------------------------------    
    
    if (dir < 0) dir += PI_M_2;
    if (PI_M_2 + PI_D_2 / 2 < dir) dir -= PI_M_2;

    fish->setDirLines(sep_lines, align_lines, coh_lines);
    fish->setRotation(sf::radians(dir));
}

std::vector<std::shared_ptr<Fish>> SimMath::getCollisions(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes,
                                             int _col_radius) {
    sf::Vector2f pos = fish->getPosition();
    std::vector<std::shared_ptr<Fish>> nearest = {};
    sf::VertexArray lines(sf::PrimitiveType::LineStrip, fishes.size() * 2);
    for (int i = 0; i < fishes.size(); i++) {
        std::shared_ptr<Fish> target = fishes.at(i);
        if ((fish.get() != target.get()) &&
            SimMath::getDistance(pos, target->getPosition()) < _col_radius) {
            sf::Vector2f sub_vec = pos - target->getPosition();

            double rad = (fmod(fish->getDirection(), PI_M_2) + PI_D_2) - atan2(sub_vec.x, -sub_vec.y);

            double rel_angle = rad < 0 ? rad + PI_M_2 : rad;

            rel_angle = rel_angle > PI_M_2 ? rel_angle - PI_M_2 : rel_angle;
            
            // std::cout << fish->name << " " << nearest.size() <<  " distance " <<  SimMath::getDistance(pos, target->getPosition()) << std::endl;
                
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
