#include <SFML/System/Vector2.hpp>
#include <cmath>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "sim_math.hpp"

#define REQUIRE_VECTOR_EQ(v1, v2, eps) \
    REQUIRE_EQ(v1.x, doctest::Approx(v2.x).epsilon(eps)); \
    REQUIRE_EQ(v1.y, doctest::Approx(v2.y).epsilon(eps))

namespace sf {
    std::ostream& operator<<(std::ostream& os, const sf::Vector2f& v) {
        os << "x: " << v.x << " y: " << v.y;
        return os;
    }
}

//NOTE The Y axis is inverted in SFML. It will not affect the method, but should be kept in mind.
namespace SimMath{
TEST_CASE("Zero Point") {
    sf::Vector2f vec {0,0};
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , 0);
}

TEST_CASE("First Quadrant") {
    sf::Vector2f vec {1,1};
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(PI_D_4).epsilon(1e-6));
}

TEST_CASE("Second Quadrant") {
    sf::Vector2f vec {-1,1};
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(PI_D_4 + PI_D_2).epsilon(1e-6));
}

TEST_CASE("Third Quadrant") {
    sf::Vector2f vec {-1,-1};
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(PI_D_4 + PI).epsilon(1e-6));
}

TEST_CASE("Fourth Quadrant") {
    sf::Vector2f vec {1,-1};
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(PI_D_4 + PI_D_2 + PI).epsilon(1e-6));
}
}
namespace SimMath{
TEST_CASE("Zero Angle") {
    float rad = 0;
    REQUIRE_EQ(SimMath::polarToCortesian(rad) , sf::Vector2f(1,0));
}

TEST_CASE("First Quadrant") {
    float rad = PI_D_4;
    REQUIRE_VECTOR_EQ(SimMath::polarToCortesian(rad) , sf::Vector2f(sqrtf(2)/2,sqrtf(2)/2), 1e-6);
}

TEST_CASE("Second Quadrant") {
    float rad = PI_D_4 + PI_D_2;
    REQUIRE_VECTOR_EQ(SimMath::polarToCortesian(rad) , sf::Vector2f(- sqrtf(2)/2,sqrtf(2)/2), 1e-6);
}

TEST_CASE("Third Quadrant") {
    float rad = PI_D_4 + PI;
    REQUIRE_VECTOR_EQ(SimMath::polarToCortesian(rad) , sf::Vector2f(- sqrtf(2)/2,- sqrtf(2)/2), 1e-6);
}

TEST_CASE("Fourth Quadrant") {
    float rad = PI_D_4 + PI_D_2 + PI;
    REQUIRE_VECTOR_EQ(SimMath::polarToCortesian(rad) , sf::Vector2f(sqrtf(2)/2, - sqrtf(2)/2), 1e-6);
}
}