#include "MenuItem.h"

#include <SDL.h>
#include <SDL_ttf.h>

MenuItem::MenuItem(SDL_Surface* screen, const std::string& label)
: ScreenObject(screen), _label(label), _selected(false)
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
