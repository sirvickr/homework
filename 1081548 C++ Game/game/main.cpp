#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Game.h"
#include "StartScreen.h"

using namespace std;

static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 600;

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(ostream &os, const string &msg)
{
	os << msg << " failed: " << SDL_GetError() << endl;
}

int main(int argc, char* argv[])
{
	// Инициализация библиотеки SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(cout, "SDL_Init");
		return 1;
	}
	// Инициализация библиотеки SDL_image
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError(cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}
	// Инициализация библиотеки SDL_ttf
	if (TTF_Init() != 0) {
		logSDLError(cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}

#if 0
	StartScreen screen(SCREEN_WIDTH, SCREEN_HEIGHT);
	screen.run();
#else
	Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
	game.run();
#endif
	// освобождение библиотек
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
