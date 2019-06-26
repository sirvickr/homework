#include "Utils.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

bool initializeGraphics()
{
	// ������������� ���������� SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {

		printf("SDL Initialization Error: %s\n", SDL_GetError());
		return false;
	}
	// ������������� ���������� SDL_image
	int flags = IMG_INIT_PNG;
	if (!(IMG_Init(flags) & flags)) {
		printf("IMG Initialization Error: %s\n", IMG_GetError());
		return false;
	}
	// ������������� ���������� SDL_ttf
	if (TTF_Init() == -1) {
		printf("TTF Initialization Error: %s\n", TTF_GetError());
		return false;
	}
	// ���������� ������� ��������� ����������
	return true;
}

void finalizeGraphics()
{
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void logSDLError(std::ostream &os, const std::string &msg)
{
	os << msg << " failed: " << SDL_GetError() << std::endl;
}
