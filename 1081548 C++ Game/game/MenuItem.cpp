#include "MenuItem.h"

#include <SDL.h>
#include <SDL_ttf.h>

MenuItem::MenuItem(SDL_Surface* screen, const SDL_Color& color, _TTF_Font* font, const std::string& label)
: Text(screen, color, font, label), _selected(false)
{
}

MenuItem::~MenuItem()
{
	clear();
}

MenuItem::Toggle MenuItem::toggle(int x, int y) {
	if (inside(x, y)) {
		if (!selected()) {
			select(true);
			return Toggle::on;
		}
	}
	else {
		if (selected()) {
			select(false);
			return Toggle::off;
		}
	}
	return Toggle::no;
}
