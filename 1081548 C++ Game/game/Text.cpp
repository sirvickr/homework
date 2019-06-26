#include "Text.h"
#include "Utils.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>

Text::Text(SDL_Surface* screen, const char* fileName, int size)
	: ScreenObject(screen), _fileName(fileName)
{
	_font = TTF_OpenFont(fileName, size);
	if(!_font) {
		logSDLError(std::cerr, "TTF_OpenFont");
	}
}

Text::~Text()
{
	if (_font) {
		TTF_CloseFont(_font);
	}
}

void Text::print(int x, int y, const std::string& text, const SDL_Color& color, SDL_Rect* dest)
{
	surface(TTF_RenderText_Blended(_font, text.c_str(), color));
	if (surface()) {
		locate(x, y);
		draw();
	}
	else {
		logSDLError(std::cerr, "renderText");
	}
}
