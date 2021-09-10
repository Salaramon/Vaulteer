#include "Game.h"

void Game::run()
{
	Event::Poll();
	if (Event::KEY::A >> Event::STATE::DOWN && Event::KEY::S >> Event::ACTION::PRESS) {
		std::cout << "wow" << std::endl;
	}

	if (
		500 < Event::CURSOR::X && Event::CURSOR::X < 700 && 
		250 < Event::CURSOR::Y && Event::CURSOR::Y < 500) {
		
		std::cout << (double_t)Event::CURSOR::X.check << std::endl;
	}
	

}
