
add_executable(test-cohesion
    test_cohesion.cpp
)
target_link_libraries(test-cohesion PRIVATE game_lib sfml-graphics sfml-window sfml-system)

add_test(NAME CohesionTest COMMAND test-cohesion)

#-----------------------------------------
add_executable(test-separation
    test_separation.cpp
)
target_link_libraries(test-separation PRIVATE game_lib sfml-graphics sfml-window sfml-system)

add_test(NAME SeparationTest COMMAND test-separation)

#-----------------------------------------
add_executable(test-alignment
    test_alignment.cpp
)

target_link_libraries(test-alignment PRIVATE game_lib sfml-graphics sfml-window sfml-system)

add_test(NAME AlignmentTest COMMAND test-alignment)

#-----------------------------------------
add_executable(test-getCollision
    test_getCollision.cpp
)
target_link_libraries(test-getCollision PRIVATE game_lib sfml-graphics sfml-window sfml-system)

add_test(NAME ColisionTest COMMAND test-getCollision)

#-----------------------------------------
include(FetchContent)
FetchContent_Declare(doctest
GIT_REPOSITORY https://github.com/doctest/doctest.git
GIT_TAG v2.4.12)
FetchContent_MakeAvailable(doctest)

add_executable(sim_math-unit-tests
    test_sim_math.cpp
)

target_link_libraries(sim_math-unit-tests PRIVATE doctest::doctest game_lib sfml-graphics sfml-window sfml-system)

add_test(NAME UnitTest COMMAND sim_math-unit-tests)

#--------------------

add_executable(test-boids
    test_boids.cpp
)
target_link_libraries(test-boids PRIVATE game_lib sfml-graphics sfml-window sfml-system)

add_test(NAME BoidsTest COMMAND test-boids)
