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
	// создаём шрифт
	_mainFont = TTF_OpenFont("res/arial.ttf", 18);
	if (!_mainFont) {
		logSDLError(std::cout, "TTF_OpenFont");
		return 3;
	}
	// текстовый объект
	SDL_Surface *txtMain = nullptr;
	// загружаем фон
	//SDL_Surface *background = IMG_Load("res/init.jpg");
	// проверяем результат
	//if (!background) {
	//	logSDLError(std::cout, "Loading images");
	//	return 4;
	//}
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

		// Отрисовка сцены

		// заполнение фона
		//SDL_BlitSurface(background, NULL, _screen, &_screen->clip_rect);
		// Текст 
		showText("User name: " + _input, 20, _scrHeight / 3 + 10, txtMain, _mainFont, { 25, 150, 150, 255 });
		// показ сцены в окне
		SDL_UpdateWindowSurface(_window);
	} // while(active)

	// Очистка ресурсов

	// освобождение ресурсов SDL

	TTF_CloseFont(_mainFont);
	_mainFont = nullptr;

	SDL_FreeSurface(txtMain);
	//SDL_FreeSurface(background);

	return 0;
}
