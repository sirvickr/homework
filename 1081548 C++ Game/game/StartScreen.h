#pragma once

#include <ostream>

#include "Window.h"

struct SDL_Window;
struct SDL_Surface;
struct SDL_Color;
struct _TTF_Font;

class StartScreen : public Window
{
public:
	StartScreen(int width, int height, const std::string& title);
	virtual ~StartScreen();
	int show(const std::string& title) override;

private:
	// графические объекты
	_TTF_Font* _mainFont = nullptr;
	_TTF_Font* _credFont = nullptr;

};

