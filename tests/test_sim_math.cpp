#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "sim_math.hpp"

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