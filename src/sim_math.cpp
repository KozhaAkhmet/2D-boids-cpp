#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ostream>
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
    //The cartesian plane is with negative y;
    float rad = atan2f(vec.y, vec.x);
    if (vec.x == 0 && vec.y == 0)
        return 0;
    return rad < 0 ? rad + PI_M_2 : rad;
}

void SimMath::updatePosition(std::shared_ptr<Fish> fish) {
    sf::Vector2f temp = fish->getPosition();

    temp.x += fish->getSpeed() * dt * cos(fish->getDirection());
    temp.y += fish->getSpeed() * dt * sin(fish->getDirection());

    fish->setPosition(temp);
}

void SimMath::separation(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes_nearby) {
    sf::Vector2f sum = {0,0};
    for (auto& n : fishes_nearby) {
        sum += (fish->getPosition() - n->getPosition())/fish->getMinDistance();
    }
    fish->setSepVec(sum);
}

void SimMath::alignment(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes_nearby) {
    if(fishes_nearby.size() > 0){
        double sum {0};
        for (auto& n : fishes_nearby) {
            sum += n->getDirection();
        }
        sum = sum / fishes_nearby.size();
        fish->setAlignAngle(sum);
    }else{
        fish->setAlignAngle(0);
    }
}

void SimMath::cohesion(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes_nearby) {
    int size = fishes_nearby.size();
    sf::Vector2f result{0,0};
    if (size > 0) {
        sf::Vector2f sum_vec{};
        for (auto& fish_nearby : fishes_nearby) {
            sum_vec += fish_nearby->getPosition();
        }
        sum_vec = sf::Vector2f(sum_vec.x / size, sum_vec.y / size);
        result = sum_vec - fish->getPosition();
    }
    fish->setCohesionVec(result);
}

float SimMath::angleToDirectionalLocalAngle(float ang_rad, float fish_dir){
    float local_ang;
    float local_ang_dir;
    local_ang = ang_rad + fish_dir;

    //after sum the angle can become too big
    local_ang = std::fmod(local_ang, PI_M_2);

    //converting to directional angle 
    local_ang_dir = local_ang > PI ? local_ang - PI_M_2: local_ang;

    return local_ang_dir;
}

float SimMath::applySeperation(std::shared_ptr<Fish> fish){
    //------------Seperation----------
    float dir = fish->getDirection();
    sf::Vector2f sep_vec = fish->getSepVec();
    //The sep_vec is the sum of fish directions.
    //sfml uses cordinate system with inverted y axis, adding "-" to y axis to shift into regular coordinate system.
    float sep_ang = cortesianToPolar(sf::Vector2f(sep_vec.x, -sep_vec.y));
    
    #ifndef NDEBUG
        sf::Vector2f fish_pos = fish->getPosition();
        sf::VertexArray sep_lines(sf::PrimitiveType::LineStrip, 2);
        sep_lines[0].position = fish_pos;
        //sfml uses cordinate system with inverted y axis, therefore adding "-" to sep_ang
        sep_lines[1].position = SimMath::polarToCortesian(-sep_ang) * fish.get()->getCollisionRadius() + fish_pos;
        sep_lines[1].color = sf::Color::Red;
        fish->setSepDirLines(sep_lines);
    #endif
    //converting sep_and to local. Adding PI to avoid angle, if removed it will steer towards it.
    return sep_const * angleToDirectionalLocalAngle(sep_ang + PI, dir);
}

float SimMath::applyAlignment(std::shared_ptr<Fish> fish){
    //------------Alignment----------
    float dir = fish->getDirection();
    //The angle has to be inverted. Not figured out yet but gives correct results on tests
    float align_ang = PI_M_2 - fish->getAlignAngle();

    #ifndef NDEBUG
        sf::Vector2f fish_pos = fish->getPosition();
        sf::VertexArray align_lines(sf::PrimitiveType::LineStrip, 2);
        align_lines[0].position = fish_pos;
        //the alignment ang is calcualted using sfml coordinate system no need to add "-" sign.
        align_lines[1].position = SimMath::polarToCortesian(-align_ang) * fish.get()->getCollisionRadius() + fish_pos;
        align_lines[1].color = sf::Color::Green;
        fish->setAlignDirLines(align_lines);
    #endif
    return align_const * angleToDirectionalLocalAngle(align_ang + PI, dir);
}

float SimMath::applyCohesion(std::shared_ptr<Fish> fish){
    //------------Cohesion----------
    float dir = fish->getDirection();
    //the cohesion vector should pass similar calculations as seperation vector
    sf::Vector2f coh_vec = fish->getCohVec();
    //sfml uses cordinate system with inverted y axis, adding "-" to y axis to shift into regular coordinate system.
    float coh_ang = cortesianToPolar(sf::Vector2f(coh_vec.x, -coh_vec.y));

    #ifndef NDEBUG
        sf::Vector2f fish_pos = fish->getPosition();
        sf::VertexArray coh_lines(sf::PrimitiveType::LineStrip, 2);
        coh_lines[0].position = fish_pos;
        //sfml uses cordinate system with inverted y axis, therefore adding "-" to coh_ang
        coh_lines[1].position =
            SimMath::polarToCortesian(-coh_ang) * fish.get()->getCollisionRadius() + fish_pos;
        coh_lines[1].color = sf::Color::Yellow;
        fish->setCohDirLines(coh_lines);
    #endif
    //converting sep_and to local. Adding PI to avoid angle, if removed it will steer towards it.
    return coh_const * angleToDirectionalLocalAngle(coh_ang + PI, dir);
}

void SimMath::applyModifiedDirection(std::shared_ptr<Fish> fish) {
    // std::cout << "##### " << std::endl << fish->name << std::endl;
    float angle = 0;

    angle += applySeperation(fish);
    angle += applyAlignment(fish);
    angle += applyCohesion(fish);

    angle = fmod(angle, PI);

    fish->setCumulativeDirection(angle);
}

std::vector<std::shared_ptr<Fish>> SimMath::getCollisions(std::shared_ptr<Fish> fish, const std::vector<std::shared_ptr<Fish>> fishes,
                                             int _col_radius) {
    sf::Vector2f pos = fish->getPosition();
    std::vector<std::shared_ptr<Fish>> nearest = {};
    #ifndef NDEBUG
        sf::VertexArray lines(sf::PrimitiveType::LineStrip, fishes.size() * 2);
    #endif
    for (int i = 0; i < fishes.size(); i++) {
        std::shared_ptr<Fish> target = fishes.at(i);
        if ((fish.get() != target.get()) &&
            SimMath::getDistance(pos, target->getPosition()) < _col_radius) 
        {
            sf::Vector2f sub_vec = pos - target->getPosition();
            //TODO rewrite these lines, correct the nameings and simplify algorithm
            double rad = (fmod(fish->getDirection(), PI_M_2) + PI_D_2) - atan2(sub_vec.x, -sub_vec.y);

            double rel_angle = rad < 0 ? rad + PI_M_2 : rad;

            rel_angle = rel_angle > PI_M_2 ? rel_angle - PI_M_2 : rel_angle;
            
            // std::cout << fish->name << " " << nearest.size() <<  " distance " <<  SimMath::getDistance(pos, target->getPosition()) << std::endl;
                
            if ((PI / 4 < rel_angle && rel_angle < 7 * PI / 4)) {
                #ifndef NDEBUG
                    lines[i * 2 + 1].position = pos;
                    lines[i * 2 + 1].color = sf::Color::Black;
                    lines[i * 2].position = target->getPosition();
                #endif

                float distance_to_target =
                    SimMath::getDistance(pos, target->getPosition());
                fish->setMinDistance( distance_to_target < fish->getMinDistance()
                                        ? distance_to_target
                                        : fish->getMinDistance());

                nearest.emplace_back(target);
            }
            //TODO bugs with debug lines they appear at 0,0 coordinates
        }
    }
    #ifndef NDEBUG
        fish->setAffectLines(lines);
    #endif

    return nearest;
}
