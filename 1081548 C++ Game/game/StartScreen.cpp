#include "StartScreen.h"
#include "Utils.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

StartScreen::StartScreen(int width, int height, const std::string& title)
:	Window(width, height, title)
{
}

int StartScreen::show()
{
	// ������ ������
	_mainFont = TTF_OpenFont("res/arial.ttf", 50);
	_credFont = TTF_OpenFont("res/arial.ttf", 18);
	if (!_mainFont || !_credFont) {
		logSDLError(std::cout, "TTF_OpenFont");
		return 3;
	}
	// ��������� �������
	SDL_Surface *txtMain = nullptr;
	SDL_Surface *txtCred = nullptr;
	// ��������� ���
	SDL_Surface *background = IMG_Load("res/init.jpg");
	// ��������� ���������
	if (!background) {
		logSDLError(std::cout, "Loading images");
		return 4;
	}
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
				stop();
				break;
			case SDL_KEYDOWN:
				switch (evt.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE:
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
		SDL_BlitSurface(background, NULL, _screen, &_screen->clip_rect);
		// ����� 
		showText(_title, 20, _scrHeight / 3 + 10, txtMain, _mainFont, { 50, 50, 200, 255 });
		// ����� 
		showText("Author: ________ Group: ___________", 20, _scrHeight / 2 + 10, txtCred, _credFont, { 30, 30, 200, 255 });
		// ����� ����� � ����
		SDL_UpdateWindowSurface(_window);
	} // while(active)

	// ������� ��������

	// ������������ �������� SDL

	TTF_CloseFont(_mainFont);
	_mainFont = nullptr;
	TTF_CloseFont(_credFont);
	_credFont = nullptr;

	SDL_FreeSurface(txtMain);
	SDL_FreeSurface(background);

	return 0;
}
