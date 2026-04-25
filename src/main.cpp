#include <SFML/Graphics.hpp>
#include <fstream>
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

	Simulation simulation{window_size.x, window_size.y};
	simulation.generate(gen, dis, num_of_fish, col_radius, speed, icon_size, dt);

	std::ofstream csv_file("framerateV2.csv");
	csv_file << "time_second,fps,fish_count\n";

	sf::Clock clock;
	int frame_count = 0;
	int last_recorded_second = -1;

	while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
			if (event->is<sf::Event::Closed>())
				window.close();
        }

        frame_count++;

        sf::Time elapsed = clock.getElapsedTime();
        int current_second = static_cast<int>(elapsed.asSeconds());

        if (current_second > last_recorded_second && current_second > 0) {
            float fps = static_cast<float>(frame_count) / (current_second - last_recorded_second);
            csv_file << current_second << "," << fps << "," << simulation.getFishes().size() << "\n";
            csv_file.flush();
            last_recorded_second = current_second;
            frame_count = 0;
			for(int i = 0 ; i < 20 ; i++)
            	simulation.addFish(gen, dis, col_radius, speed, icon_size, dt);

            if (current_second >= 60) {
                window.close();
                break;
            }
        }

		window.clear();
		simulation.run(window);
		window.display();
	}

	csv_file.close();
	return 0;
}
