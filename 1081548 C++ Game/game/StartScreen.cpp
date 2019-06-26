#include "StartScreen.h"
#include "Graphics.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

using namespace std;

StartScreen::StartScreen(int width, int height, const string& title)
:	Window(width, height, title)
{
}

StartScreen::~StartScreen()
{
}

int StartScreen::show(const string& title)
{
	Window::show(title);
	// создаём шрифты
	_mainFont = TTF_OpenFont("res/arial.ttf", 50);
	_credFont = TTF_OpenFont("res/arial.ttf", 18);
	if (!_mainFont || !_credFont) {
		Graphics::logSDLError(cout, "TTF_OpenFont");
		return 3;
	}
	// текстовые объекты
	SDL_Surface *txtMain = nullptr;
	SDL_Surface *txtCred = nullptr;
	// загружаем фон
	SDL_Surface *background = IMG_Load("res/init.jpg");
	// проверяем результат
	if (!background) {
		Graphics::logSDLError(cout, "Loading images");
		return 4;
	}
	// размеры картинки с прицелом
	// событие SDL
	SDL_Event evt;
	// время одного шага - 50 мс
	constexpr uint32_t stepDelay = 50; // ms
	// цикл обработки событий
	while (_active) {
		SDL_Delay(stepDelay);
		// Обработка событий пользователя
		while (SDL_PollEvent(&evt)) {
			switch (evt.type) {
			case SDL_QUIT:
				stop();
				break;
			case SDL_KEYDOWN:
				switch (evt.key.keysym.scancode)
				{
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

		// Отрисовка сцены

		// заполнение фона
		SDL_BlitSurface(background, NULL, _screen, &_screen->clip_rect);
		// Текст 
		showText(Graphics::Instance()->WINDOW_TITLE, 20, _scrHeight / 3 + 10, txtMain, _mainFont, { 50, 50, 200, 255 });
		// Текст 
		showText(Graphics::Instance()->AUTHOR, 20, _scrHeight / 2 + 10, txtCred, _credFont, { 30, 30, 200, 255 });
		// показ сцены в окне
		SDL_UpdateWindowSurface(_window);
	} // while(active)

	// Очистка ресурсов

	// освобождение ресурсов SDL

	TTF_CloseFont(_mainFont);
	_mainFont = nullptr;
	TTF_CloseFont(_credFont);
	_credFont = nullptr;

	SDL_FreeSurface(txtMain);
	SDL_FreeSurface(background);

	return 0;
}
