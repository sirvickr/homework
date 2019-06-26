#include "Text.h"
#include "Utils.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>

Text::Text(SDL_Surface* screen, const SDL_Color& color, const char* fileName, int size, const std::string& txt)
	: ScreenObject(screen), _ownsFont(true), _font(nullptr), _color(color)
{
	_font = TTF_OpenFont(fileName, size);
	if (_font) {
		text(txt);
	}
	else {
		logSDLError(std::cerr, "TTF_OpenFont");
	}
}

Text::Text(SDL_Surface* screen, const SDL_Color& color, _TTF_Font* font, const std::string& txt)
	: ScreenObject(screen), _ownsFont(false), _font(font), _color(color)
{
	if (_font) {
		text(txt);
	}
}

Text::~Text()
{
	if (_font && _ownsFont) {
		TTF_CloseFont(_font);
		_font = nullptr;
	}
}

void Text::color(const SDL_Color& value) {
	_color = value;
	surface(TTF_RenderText_Blended(_font, _text.c_str(), _color));
}

void Text::text(const std::string& value) {
	_text = value;
	surface(TTF_RenderText_Blended(_font, _text.c_str(), _color));
}
