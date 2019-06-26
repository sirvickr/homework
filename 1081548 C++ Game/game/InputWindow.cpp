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
			} // case
		} // while

		// Текст 
		txtMain->text("User name: " + _input);
		// Отрисовка сцены
		for (auto it = begin(objects); it != end(objects); it++) {
			(*it)->draw();
		}
		// показ сцены в окне
		SDL_UpdateWindowSurface(_window);
	} // while(active)

	// Очистка ресурсов

	for (auto it = begin(objects); it != end(objects); it++) {
		delete (*it);
	}

	return 0;
}
