#include "StartScreen.h"
#include "Utils.h"
#include "Image.h"
#include "Text.h"

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
	// ��������� �������
	Text *txtMain = new Text(_screen, "res/arial.ttf", 40, { 50, 50, 200, 255 });
	Text *txtAuthor = new Text(_screen, "res/arial.ttf", 18, { 30, 30, 200, 255 });
	if (!txtMain || !txtAuthor) {
		return 3;
	}
	txtMain->locate(20, _scrHeight / 3 + 10);
	txtMain->text(_title);

	txtAuthor->locate(20, _scrHeight / 2 + 10);
	txtAuthor->text("Author: ________ Group: ___________");
	// ��������� ���
	objects.push_back(new Image(_screen, "res/init.jpg"));
	objects.push_back(txtMain);
	objects.push_back(txtAuthor);
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
