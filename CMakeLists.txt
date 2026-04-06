cmake_minimum_required(VERSION 3.10)
project(Boids VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(2d-boids-cpp
    src/main.cpp
)
add_library(game_lib     
    src/fish.cpp
    src/fish.hpp
    src/sim_math.cpp
    src/sim_math.hpp
    src/simulation.hpp
    src/simulation.cpp
)

add_custom_command(TARGET 2d-boids-cpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res
    $<TARGET_FILE_DIR:2d-boids-cpp>/res
)

target_link_libraries(2d-boids-cpp PRIVATE game_lib sfml-graphics sfml-window sfml-system)

enable_testing()
add_subdirectory(tests)