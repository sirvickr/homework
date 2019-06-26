#include "InputWindow.h"
#include "Utils.h"

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
	// ������ �����
	_mainFont = TTF_OpenFont("res/arial.ttf", 18);
	if (!_mainFont) {
		logSDLError(std::cout, "TTF_OpenFont");
		return 3;
	}
	// ��������� ������
	SDL_Surface *txtMain = nullptr;
	// ��������� ���
	//SDL_Surface *background = IMG_Load("res/init.jpg");
	// ��������� ���������
	//if (!background) {
	//	logSDLError(std::cout, "Loading images");
	//	return 4;
	//}
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
			case SDL_MOUSEBUTTONDOWN:
				stop();
				break;
			} // case
		} // while

		// ��������� �����

		// ���������� ����
		//SDL_BlitSurface(background, NULL, _screen, &_screen->clip_rect);
		// ����� 
		showText("User name: " + _input, 20, _scrHeight / 3 + 10, txtMain, _mainFont, { 25, 150, 150, 255 });
		// ����� ����� � ����
		SDL_UpdateWindowSurface(_window);
	} // while(active)

	// ������� ��������

	// ������������ �������� SDL

	TTF_CloseFont(_mainFont);
	_mainFont = nullptr;

	SDL_FreeSurface(txtMain);
	//SDL_FreeSurface(background);

	return 0;
}
