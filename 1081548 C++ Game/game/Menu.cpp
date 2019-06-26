#include "Menu.h"
#include "MenuItem.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>

Menu::Menu(SDL_Window* window)
:	_window(window)
{
}

Menu::~Menu()
{
}

int Menu::show(SDL_Surface* screen, _TTF_Font* font, const Labels& labels)
{
	if (labels.empty()) {
		return -2;
	}
	// normal, selected
	const SDL_Color color[2] = { { 255, 255, 255 }, { 255, 0, 0 } };
	Items items;
	if (labels.empty()) {
		return -1;
	}
	items.reserve(labels.size());
	for (const auto& label : labels) {
		items.push_back(new MenuItem(screen, color[0], font, label));
	}
	items[0]->select(true);
	uint32_t time;

	int height = 0;
	items[0]->color(color[1]);
	for (int i = 0; i < items.size(); ++i) {
		items[i]->draw();
		height += items[i]->h();
	}
	int top = (screen->clip_rect.h - height) / 2;
	int h = items[0]->surface()->clip_rect.h;
	int left = (screen->clip_rect.w - items[0]->w()) / 2;

	for (int i = 0; i < items.size(); ++i) {
		items[i]->locate(left, top);
		top += h;
	}
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

	SDL_Event event;
	while (true) {
		time = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				clear(items);
				return -1;
			case SDL_MOUSEMOTION:
				for (auto item : items) {
					switch (item->toggle(event.motion.x, event.motion.y)) {
					case MenuItem::Toggle::on:
						item->color(color[1]);
						item->draw();
						break;
					case MenuItem::Toggle::off:
						item->color(color[0]);
						item->draw();
						break;
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				for (size_t i = 0; i < items.size(); ++i) {
					if (items[i]->inside(event.button.x, event.button.y)) {
						clear(items);
						return static_cast<int>(i);
					}
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					clear(items);
					return -1;
				}
				break;
			}
		}
		for (int i = 0; i < items.size(); ++i) {
			items[i]->draw();
		}
		SDL_UpdateWindowSurface(_window);
		SDL_Delay(20);
	} // while
	clear(items);
	return 0;
}

void Menu::clear(Items& items) {
	for (MenuItem* item : items) {
		item->clear();
		delete item;
	}
	items.clear();
}
