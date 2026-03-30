cmake_minimum_required(VERSION 3.10)
project(Boids VERSION 1.0 LANGUAGES CXX)

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

target_link_libraries(sfml-app PRIVATE sfml-graphics sfml-window sfml-system)
