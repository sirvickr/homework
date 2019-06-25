#include "MenuItem.h"

#include <SDL.h>
#include <SDL_ttf.h>

MenuItem::MenuItem(const std::string& label)
: _label(label), _selected(false), _surface(nullptr)
{
}

MenuItem::~MenuItem()
{
	clear();
}

void MenuItem::verify(int x, int y, _TTF_Font* font, const SDL_Color& clrOn, const SDL_Color& clrOff) {
	if (inside(x, y)) {
		if (!selected()) {
			select(true);
			surface(TTF_RenderText_Blended(font, _label.c_str(), clrOn));
		}
	}
	else {
		if (selected()) {
			select(false);
			surface(TTF_RenderText_Blended(font, _label.c_str(), clrOff));
		}
	}
}

void MenuItem::blit(SDL_Surface* screen) {
	SDL_BlitSurface(_surface, NULL, screen, &_pos);
}

void MenuItem::clear() {
	if (_surface) {
		SDL_FreeSurface(_surface);
		_surface = nullptr;
	}
}

void MenuItem::surface(SDL_Surface* value) {
	clear();
	_surface = value;
}
