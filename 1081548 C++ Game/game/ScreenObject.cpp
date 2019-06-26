#include "ScreenObject.h"

#include <SDL.h>

ScreenObject::ScreenObject(SDL_Surface* screen): _screen(screen), _surface(nullptr)
{
}

ScreenObject::~ScreenObject()
{
	clear();
}

void ScreenObject::surface(SDL_Surface* value) {
	clear();
	_surface = value;
	if (_surface) {
		_pos.w = _surface->clip_rect.w;
		_pos.h = _surface->clip_rect.h;
	}
}

void ScreenObject::draw(SDL_Rect* dest)
{
	if (_surface) {
		if (dest) {
			SDL_BlitSurface(_surface, NULL, _screen, dest);
		}
		else {
			SDL_BlitSurface(_surface, NULL, _screen, &_pos);
		}
	}
}

void ScreenObject::clear() {
	if (_surface) {
		SDL_FreeSurface(_surface);
		_surface = nullptr;
	}
}
