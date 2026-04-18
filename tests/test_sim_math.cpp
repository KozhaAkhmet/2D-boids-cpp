#include <SFML/System/Vector2.hpp>
#include <cmath>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "sim_math.hpp"
#include "consts.hpp"

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

TEST_CASE("Positive X Axis") {
    sf::Vector2f vec {1,0};
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(0).epsilon(1e-6));
}

TEST_CASE("Negative X Axis") {
    sf::Vector2f vec {-1,0};
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(PI).epsilon(1e-6));
}

TEST_CASE("Positive Y Axis") {
    sf::Vector2f vec {0,1};
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(PI_D_2).epsilon(1e-6));
}

TEST_CASE("Large Distance") {
    sf::Vector2f vec {100,100};
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(PI_D_4).epsilon(1e-6));
}

TEST_CASE("Small Distance") {
    sf::Vector2f vec {0.001f,0.001f};
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(PI_D_4).epsilon(1e-6));
}

TEST_CASE("Angle PI/6") {
    sf::Vector2f vec {sqrtf(3), 1};
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(PI / 6).epsilon(1e-6));
}

TEST_CASE("Angle PI/3") {
    sf::Vector2f vec {1, sqrtf(3)};
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(PI / 3).epsilon(1e-6));
}

TEST_CASE("Angle 5PI/4 - Third Quadrant") {
    sf::Vector2f vec {-1, -1};
    float angle = 5 * PI / 4;
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(angle).epsilon(1e-6));
}

TEST_CASE("Angle 3PI/4 - Second Quadrant") {
    sf::Vector2f vec {-0.5f, 0.5f};
    float angle = 3 * PI / 4;
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(angle).epsilon(1e-6));
}

TEST_CASE("Angle 7PI/4 - Fourth Quadrant") {
    sf::Vector2f vec {0.5f, -0.5f};
    float angle = 7 * PI / 4;
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(angle).epsilon(1e-6));
}

TEST_CASE("Angle 11PI/6 - Near X Axis") {
    sf::Vector2f vec {sqrtf(3), -1};
    float angle = 11 * PI / 6;
    REQUIRE_EQ(SimMath::cortesianToPolar(vec) , doctest::Approx(angle).epsilon(1e-6));
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

TEST_CASE("Angle PI/12 - Near X Axis") {
    float rad = PI / 12;
    REQUIRE_VECTOR_EQ(SimMath::polarToCortesian(rad), sf::Vector2f(cosf(rad), sinf(rad)), 1e-3);
}

TEST_CASE("Angle 5PI/12 - First Quadrant") {
    float rad = 5 * PI / 12;
    REQUIRE_VECTOR_EQ(SimMath::polarToCortesian(rad), sf::Vector2f(cosf(rad), sinf(rad)), 1e-3);
}

TEST_CASE("Angle 7PI/12 - Second Quadrant") {
    float rad = 7 * PI / 12;
    REQUIRE_VECTOR_EQ(SimMath::polarToCortesian(rad), sf::Vector2f(cosf(rad), sinf(rad)), 1e-3);
}

TEST_CASE("Angle 11PI/6 - Near X Axis Negative") {
    float rad = 11 * PI / 6;
    REQUIRE_VECTOR_EQ(SimMath::polarToCortesian(rad), sf::Vector2f(cosf(rad), sinf(rad)), 1e-3);
}
}

// TEST_CASE("Angle 5PI/12 - First Quadrant") {
//     float rad = 5 * PI / 12;
//     sf::Vector2f expected {1.366f, 2.732f};
//     float len = sqrtf(expected.x * expected.x + expected.y * expected.y);
//     expected /= len;
//     REQUIRE_VECTOR_EQ(SimMath::polarToCortesian(rad), expected, 1e-3);
// }

// TEST_CASE("Angle 7PI/12 - Second Quadrant") {
//     float rad = 7 * PI / 12;
//     sf::Vector2f expected {-1.035f, 0.483f};
//     float len = sqrtf(expected.x * expected.x + expected.y * expected.y);
//     expected /= len;
//     REQUIRE_VECTOR_EQ(SimMath::polarToCortesian(rad), expected, 1e-3);
// }

// TEST_CASE("Angle 11PI/6 - Near X Axis Negative") {
//     float rad = 11 * PI / 6;
//     sf::Vector2f expected {sqrtf(3), -1};
//     float len = sqrtf(expected.x * expected.x + expected.y * expected.y);
//     expected /= len;
//     REQUIRE_VECTOR_EQ(SimMath::polarToCortesian(rad), expected, 1e-3);
// }
// }