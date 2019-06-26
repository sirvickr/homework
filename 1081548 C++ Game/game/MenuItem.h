#pragma once

#include <vector>
#include <string>

#include "ScreenObject.h"

struct _TTF_Font;

class MenuItem : public ScreenObject
{
public:
	MenuItem(SDL_Surface* screen, const std::string& label);
	~MenuItem();
	void verify(int x, int y, _TTF_Font* font, const SDL_Color& clrOn, const SDL_Color& clrOff);
	std::string label() const {
		return _label;
	}
	bool selected() const {
		return _selected;
	}
	void select(bool value) {
		_selected = value;
	}
private:
	std::string _label = "";
	bool _selected = false;
};
