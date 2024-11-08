all:
	g++ fish.hpp fish.cpp main.cpp -o fish -lsfml-graphics -lsfml-window -lsfml-system
clean:
	rm fish
