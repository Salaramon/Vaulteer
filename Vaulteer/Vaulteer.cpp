
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <future>

#include "Game.h"

#include "Event.h"

int main() {

	using namespace std::chrono_literals;
	glfwInit();
	
	Game game;

	std::future gameThread = std::async(std::launch::async, &Game::run, &game);
	
	while (!game.done) {

	}
	glfwMakeContextCurrent(game.window->getRawWindow());
	Event::AddEventHandlingForWindow(game.window);

	while (gameThread.wait_for(0ms) != std::future_status::ready) {
		glfwPollEvents();
		std::cout << "oh.. no....." << std::endl;
	}
	Event::Poll();
	if (Event::Check << (Event::Key == Event::KEY::A && Event::Action == Event::ACTION::PRESS)) {
		std::cout << "worked! maybe..." << std::endl;
	}

	return 0;
}

//TODO
/*
Import and modify TileMap
Event handling

Make camera and make it able to rotate around the y axis
*/

