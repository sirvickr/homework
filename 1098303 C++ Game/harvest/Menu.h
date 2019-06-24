#pragma once

#include <vector>
#include <string>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Color;
struct _TTF_Font;

class MenuItem;

class Menu
{
public:
	using Labels = std::vector<std::string>;
	using Items = std::vector<MenuItem*>;
	Menu(SDL_Window* window, SDL_Renderer* renderer);
	~Menu();
	int show(SDL_Surface* screen, _TTF_Font* font, const Labels& labels);
	void clear(Items& items);
private:
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
};

