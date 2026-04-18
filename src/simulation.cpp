#include <filesystem>
#include <memory>

#include "simulation.hpp"
#include "consts.hpp"
#include "fish.hpp"
#include "sim_math.hpp"

Simulation::Simulation(unsigned int _window_size_x, unsigned int _window_size_y)
    : window_size_x(_window_size_x), window_size_y(_window_size_y) {
    for (const auto& file : std::filesystem::directory_iterator(SimMath::fish_icons_dir_path)) {
		sf::Texture t;
		t.loadFromFile(file.path());
		t.setSmooth(true);
		t.generateMipmap();
		imgmap.push_back(t);
    }
}

void Simulation::run(sf::RenderWindow& window){
    for (auto fish : fishes) {
		auto fishes_nearby = SimMath::getCollisions(fish, fishes, SimMath::col_radius);
		SimMath::updatePosition(fish);
		checkBoundries(*fish);
		if(fishes_nearby.size() > 0){
			SimMath::separation(fish, fishes_nearby);
			SimMath::alignment(fish, fishes_nearby);
			SimMath::cohesion(fish, fishes_nearby);
			SimMath::applyModifiedDirection(fish);
		}
		window.draw(*fish);
    }
}

void Simulation::display(sf::RenderWindow& _window) {
    for (auto& fish : fishes) {
		_window.draw(*fish);
    }
}

void Simulation::generate(std::mt19937& gen,
                        std::uniform_real_distribution<float> dis,
                        int number_of_fish, int col_radius, float speed,
                        float radius, float dt) {
    for (int i = 0; i < number_of_fish; i++) {
		fishes.emplace_back(std::make_shared<Fish>(col_radius * 5, speed, radius, (dis(gen) * SimMath::PI_M_2),
								dt, (dis(gen) * window_size_x),
								(dis(gen) * window_size_y)));
		fishes[i]->setTexture(&imgmap[3]);
    }
}

void Simulation::checkBoundries(Fish& fish) {
    sf::Vector2f temp = fish.getPosition();
    if (temp.x > window_size_x)
      	temp.x = 0;
    else if (temp.x < 0)
      	temp.x = window_size_x;
    if (temp.y > window_size_y)
      	temp.y = 0;
    else if (temp.y < 0)
      	temp.y = window_size_y;
    fish.setPosition(temp);
}
