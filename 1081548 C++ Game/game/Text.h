#pragma once

#include "ScreenObject.h"

#include <string>

struct _TTF_Font;

class Text : public ScreenObject
{
public:
	Text(SDL_Surface* screen, const char* fileName, int size);
	~Text();
	std::string fileName() const {
		return _fileName;
	}
	void print(int x, int y, const std::string& text, const SDL_Color& color, SDL_Rect* dest = nullptr);

private:
	// מבתוךע רנטפעא
	_TTF_Font* _font = nullptr;
	// טלÿ פאיכא ס רנטפעמל
	std::string _fileName;
};

