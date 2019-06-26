#pragma once

#include "Window.h"

struct SDL_Window;
struct SDL_Surface;
struct SDL_Color;

class InputWindow: public Window
{
public:
	InputWindow(int width, int height, const std::string& title);
	~InputWindow() = default;
	int show() override;
	bool confirmed() const {
		return _confirmed;
	}
	std::string input() const {
		return _input;
	}

private:
	// ������� �������������
	bool _confirmed = false;
	// �������� ������
	std::string _input = "";
};

