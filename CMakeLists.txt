cmake_minimum_required(VERSION 3.10)
project(Boids VERSION 1.0 LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(sfml-app
    src/main.cpp
    src/fish.cpp
    src/fish.hpp
    src/sim_math.hpp
    src/simulation.hpp
    src/simulation.cpp
)

add_custom_command(TARGET sfml-app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res
    $<TARGET_FILE_DIR:sfml-app>/res
)

target_link_libraries(sfml-app PRIVATE sfml-graphics sfml-window sfml-system)

find_package(doctest QUIET)

add_executable(test-direction
    tests/test_getCollision.cpp
    src/fish.cpp
    src/fish.hpp
    src/sim_math.hpp
    src/simulation.hpp
    src/simulation.cpp
)

target_link_libraries(test-direction PRIVATE sfml-graphics sfml-window sfml-system)

if(doctest_FOUND)
    target_link_libraries(test-direction PRIVATE doctest::doctest)
    target_compile_definitions(test-direction PRIVATE DOCTEST_CONFIG_SUPER_STACK_CANARY)
endif()

add_test(NAME DirectionTest COMMAND test-direction)