#pragma once

#include "Window.h"

struct SDL_Window;
struct SDL_Surface;
struct SDL_Color;

class StartScreen : public Window
{
public:
	StartScreen(int width, int height, const std::string& title);
	~StartScreen() = default;
	int show() override;
};

