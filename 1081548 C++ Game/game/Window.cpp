#include "Window.h"
#include "Graphics.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

using namespace std;

Window::Window(int width, int height, const string& title)
	: _active(true)
{
	_scrWidth = width;
	_scrHeight = height;
	_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, _scrWidth, _scrHeight + 50, SDL_WINDOW_SHOWN);
	if (_window) {
		_screen = SDL_GetWindowSurface(_window);
	}
}

Window::~Window()
{
	SDL_DestroyWindow(_window);
	_window = nullptr;
	_screen = nullptr;
}

void Window::stop()
{
	_active = false;
}

void Window::showText(const std::string& text, int x, int y, SDL_Surface* surface, _TTF_Font* font, const SDL_Color& color)
{
	if (surface) {
		SDL_FreeSurface(surface);
	}
	surface = TTF_RenderText_Blended(font, text.c_str(), color);
	if (surface) {
		SDL_Rect dest = { x, y, surface->clip_rect.w, surface->clip_rect.h };
		SDL_BlitSurface(surface, NULL, _screen, &dest);
	}
	else {
		Graphics::logSDLError(cout, "renderText");
	}
}
