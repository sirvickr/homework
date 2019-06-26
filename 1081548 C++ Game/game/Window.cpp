#include "Window.h"
#include "Utils.h"
#include "ScreenObject.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

using namespace std;

Window::Window(int width, int height, const string& title)
	: _active(true), _title(title)
{
	_scrWidth = width;
	_scrHeight = height;
	_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, _scrWidth, _scrHeight, SDL_WINDOW_SHOWN);
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
