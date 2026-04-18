#include <SFML/Graphics.hpp>
#include <random>

#include "simulation.hpp"
#include "consts.hpp"

using namespace SimMath;

int main() {
	sf::RenderWindow window(sf::VideoMode(window_size),
													"Boids Algorothm");
	window.setFramerateLimit(max_framerate);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	// --- Setup ---
	Simulation simulation{window_size.x, window_size.y};
	simulation.generate(gen, dis, num_of_fish, col_radius, speed, icon_size, dt);

	// --- Main Loop ---
	while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
			if (event->is<sf::Event::Closed>())
				window.close();
        }
 
		window.clear();
		simulation.run(window);
		window.display();
	}
	return 0;
}
