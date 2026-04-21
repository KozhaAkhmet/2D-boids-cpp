#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>

#include "consts.hpp"
#include "fish.hpp"
#include "sim_math.hpp"

int main() {
	const float col_radius = 100;
	const int num_of_fish = 200;
	const sf::Vector2u window_size(300,300);
	const int max_framerate = 60;
	const float speed = 80.F;
	const float dt = 0.0069444445F * 4;


	sf::RenderWindow window(sf::VideoMode(window_size),
	"test_separation");
	window.setFramerateLimit(max_framerate);

	std::vector<sf::Texture> imgmap;
	for (const auto& file : std::filesystem::directory_iterator(SimMath::fish_icons_dir_path)) {
	sf::Texture t;
	t.loadFromFile(file.path());
	t.setSmooth(true);
	t.generateMipmap();
	imgmap.push_back(t);
	}

	Fish dummy_fish(col_radius, speed, SimMath::icon_size, 0.0001 + SimMath::PI_D_4,
								dt, window_size.x/2,
								window_size.y/2);

	dummy_fish.setTexture(&imgmap[3]);

	Fish cursorFish(col_radius, speed, SimMath::icon_size, SimMath::PI + SimMath::PI_D_2,
								dt, window_size.x/2 - 20,
								window_size.y/2);

	auto dummy_ptr = std::make_shared<Fish>(dummy_fish);
	auto cursor_ptr = std::make_shared<Fish>(cursorFish);

	std::vector<std::shared_ptr<Fish>> test_fishes = {dummy_ptr, cursor_ptr};

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}
		window.clear();

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
		if(worldPos.x<0)
			worldPos.x = 0;
		if(worldPos.y<0)
			worldPos.y = 0;
		cursor_ptr->setPosition(worldPos);
		auto fishes_nearby_dummy = SimMath::getCollisions(dummy_ptr, test_fishes, SimMath::col_radius);
		auto fishes_nearby_cursor = SimMath::getCollisions(cursor_ptr, test_fishes, SimMath::col_radius);
		SimMath::separation(dummy_ptr, fishes_nearby_dummy);
		SimMath::separation(cursor_ptr, fishes_nearby_cursor);
		if(fishes_nearby_dummy.size() > 0)
			SimMath::applyModifiedDirection(dummy_ptr);
		if(fishes_nearby_cursor.size() > 0)
			SimMath::applyModifiedDirection(cursor_ptr);

		#ifndef NDEBUG
			dummy_ptr->drawCollisionDebug(window);
			cursor_ptr->drawCollisionDebug(window);
		#endif
		window.draw(*cursor_ptr);
		window.draw(*dummy_ptr);

		window.display();
	}
	return 0;
}
