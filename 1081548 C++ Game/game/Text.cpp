#include "Text.h"
#include "Utils.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>

Text::Text(SDL_Surface* screen, const char* fileName, int size, const SDL_Color& color)
	: ScreenObject(screen), _fileName(fileName), _color(color)
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

void Text::text(const std::string& value) {
	_text = value;
	surface(TTF_RenderText_Blended(_font, _text.c_str(), _color));
}
