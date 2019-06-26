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
	Items items;
	if (labels.empty()) {
		return -1;
	}
	items.reserve(labels.size());
	for (const auto& label : labels) {
		items.push_back(new MenuItem(label));
	}
	items[0]->select(true);
	// normal, selected
	SDL_Color color[2] = { { 255, 255, 255 }, { 255, 0, 0 } };
	uint32_t time;

	int height = 0;
	for (int i = 0; i < items.size(); ++i) {
		items[i]->surface(TTF_RenderText_Blended(font, items[i]->label().c_str(), color[i == 0 ? 1 : 0]));
		height += items[i]->surface()->clip_rect.h;
	}
	int top = (screen->clip_rect.h - height) / 2;
	int h = items[0]->surface()->clip_rect.h;
	int left = (screen->clip_rect.w - items[0]->surface()->clip_rect.w) / 2;

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
					item->verify(event.motion.x, event.motion.y, font, color[1], color[0]);
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
			items[i]->blit(screen);
		}
		SDL_UpdateWindowSurface(_window);
		//SDL_Flip(screen);
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
