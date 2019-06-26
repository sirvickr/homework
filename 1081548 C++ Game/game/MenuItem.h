#pragma once

#include <vector>
#include <string>

#include "Text.h"

class MenuItem : public Text
{
public:
	enum class Toggle : int {
		no, on, off
	};
	MenuItem(SDL_Surface* screen, const SDL_Color& color, _TTF_Font* font, const std::string& label);
	~MenuItem();
	// сменять выделение?
	Toggle toggle(int x, int y);
	bool selected() const {
		return _selected;
	}
	void select(bool value) {
		_selected = value;
	}
private:
	bool _selected = false;
};
