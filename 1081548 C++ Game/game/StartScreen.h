#pragma once

#include "Window.h"

struct SDL_Window;
struct SDL_Surface;
struct SDL_Color;
struct _TTF_Font;

class StartScreen : public Window
{
public:
	StartScreen(int width, int height, const std::string& title);
	~StartScreen() = default;
	int show() override;

private:
	// графические объекты
	_TTF_Font* _mainFont = nullptr;
	_TTF_Font* _credFont = nullptr;

};

