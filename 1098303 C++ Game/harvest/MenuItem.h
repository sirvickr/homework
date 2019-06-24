#pragma once

#include <vector>
#include <string>
#include <SDL_rect.h>

struct SDL_Surface;
struct SDL_Color;
struct _TTF_Font;

class MenuItem
{
public:
	MenuItem(const std::string& label);
	~MenuItem();
	void verify(int x, int y, _TTF_Font* font, const SDL_Color& clrOn, const SDL_Color& clrOff);
	void blit(SDL_Surface* screen);
	void clear();
	void surface(SDL_Surface* value);
	SDL_Surface* surface() const {
		return _surface;
	}
	std::string label() const {
		return _label;
	}
	bool inside(int x, int y) const {
		return (x >= _pos.x && x <= _pos.x + _pos.w && y >= _pos.y && y <= _pos.y + _pos.h);
	}
	bool selected() const {
		return _selected;
	}
	void select(bool value) {
		_selected = value;
	}
	SDL_Rect pos() const {
		return _pos;
	}
	void locate(int x, int y) {
		_pos.x = x;
		_pos.y = y;
	}
private:
	std::string _label = "";
	bool _selected = false;
	SDL_Surface* _surface = nullptr;
	SDL_Rect _pos = { 0, 0, 0, 0 };
};
