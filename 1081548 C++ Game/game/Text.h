#pragma once

#include "ScreenObject.h"

#include <string>

struct _TTF_Font;

class Text : public ScreenObject
{
public:
	Text(SDL_Surface* screen, const char* fileName, int size, const SDL_Color& color);
	~Text();
	std::string fileName() const {
		return _fileName;
	}
	SDL_Color color() const {
		return _color;
	}
	void color(const SDL_Color& value) {
		_color = value;
	}
	std::string text() const {
		return _text;
	}
	void text(const std::string& value);

private:
	// מבתוךע רנטפעא
	_TTF_Font* _font = nullptr;
	// טלÿ פאיכא ס רנטפעמל
	std::string _fileName;
	// צגוע רנטפעא
	SDL_Color _color;
	std::string _text;
};

