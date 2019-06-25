#include <iostream>
#include "Game.h"

static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[])
{
	Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
	game.run();
	return 0;
}
