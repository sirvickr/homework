#include "InputWindow.h"
#include "Utils.h"
#include "Text.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

InputWindow::InputWindow(int width, int height, const std::string& title)
	: Window(width, height, title)
{
}

int InputWindow::show()
{
	SDL_FillRect(_screen, &_screen->clip_rect, SDL_MapRGB(_screen->format, 50, 50, 50));
	Text *txtMain = new Text(_screen, { 25, 150, 150, 255 }, "res/arial.ttf", 20);
	if (!txtMain) {
		return 3;
	}
	txtMain->locate(20, _scrHeight / 3 + 10);
	objects.push_back(txtMain);
	// ������� SDL
	SDL_Event evt;
	// ����� ������ ���� - 50 ��
	constexpr uint32_t stepDelay = 50; // ms
	// ���� ��������� �������
	while (_active) {
		SDL_Delay(stepDelay);
		// ��������� ������� ������������
		while (SDL_PollEvent(&evt)) {
			switch (evt.type) {
			case SDL_QUIT:
				std::cout << "SDL_QUIT" << std::endl;
				stop();
				break;
			case SDL_TEXTINPUT:
				_input += evt.text.text;
				break;
			case SDL_KEYDOWN:
				switch (evt.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE:
					_confirmed = false;
					stop();
					break;
				case SDL_SCANCODE_RETURN:
					_confirmed = true;
					stop();
					break;
				}
				break;
			} // case
		} // while

		// ����� 
		txtMain->text("User name: " + _input);
		// ��������� �����
		for (auto it = begin(objects); it != end(objects); it++) {
			(*it)->draw();
		}
		// ����� ����� � ����
		SDL_UpdateWindowSurface(_window);
	} // while(active)

	// ������� ��������

	for (auto it = begin(objects); it != end(objects); it++) {
		delete (*it);
	}

	return 0;
}
