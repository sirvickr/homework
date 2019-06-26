#pragma once

#include "ScreenObject.h"

#include <string>

struct _TTF_Font;

class Text : public ScreenObject
{
public:
	Text(SDL_Surface* screen, const SDL_Color& color, const char* fileName, int size, const std::string& txt = "");
	Text(SDL_Surface* screen, const SDL_Color& color, _TTF_Font* font, const std::string& txt = "");
	~Text();
	SDL_Color color() const {
		return _color;
	}
	void color(const SDL_Color& value);
	void text(const std::string& value);
	std::string text() const {
		return _text;
	}

private:
	// признак владения шрифтом
	bool _ownsFont;
	// объект шрифта
	_TTF_Font* _font = nullptr;
	// цвет шрифта
	SDL_Color _color;
	// строка текста
	std::string _text;
};

