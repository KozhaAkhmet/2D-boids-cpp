cmake_minimum_required(VERSION 3.28)
project(Boids VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build")
else()
    message(STATUS "Release build")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(2d-boids-cpp
    src/main.cpp
)
add_library(game_lib     
    src/fish.cpp
    src/fish.hpp
    src/consts.hpp
    src/sim_math.cpp
    src/sim_math.hpp
    src/simulation.hpp
    src/simulation.cpp
)

target_include_directories(game_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(game_lib PRIVATE SFML::Graphics SFML::Window SFML::System)

#TODO this command is not consistent
add_custom_command(TARGET 2d-boids-cpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res
    $<TARGET_FILE_DIR:2d-boids-cpp>/res
)

target_link_libraries(2d-boids-cpp PRIVATE game_lib SFML::Graphics SFML::Window SFML::System)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 2d-boids-cpp)

enable_testing()
add_subdirectory(tests)