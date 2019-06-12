#include "Game.h"

Game::Game(): active(true)
{
}


Game::~Game()
{
}

int Game::run()
{
	while (active)
	{

	}
	return 0;
}

void Game::stop()
{
	active = false;
}
