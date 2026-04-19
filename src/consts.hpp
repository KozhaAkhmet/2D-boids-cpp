#pragma once
#include <SFML/Graphics.hpp>
#include <string>
namespace SimMath {
    const std::string fish_icons_dir_path = "../res";
    constexpr float icon_size = 10;
    constexpr float col_radius = 100;
    constexpr int num_of_fish = 200;
    const sf::Vector2u window_size(1000,1000);
    constexpr int max_framerate = 60;
    constexpr float speed = 80.F;
    
    constexpr float PI = 3.1415926F;
    constexpr float PI_M_2 = PI * 2;
    constexpr float PI_D_2 = PI / 2;
    constexpr float PI_D_4 = PI / 4;
    
    constexpr float dt = 0.0069444F * 4;

    const float sep_const   = 1;
    const float align_const = 1;
    const float coh_const   = 1;
}
