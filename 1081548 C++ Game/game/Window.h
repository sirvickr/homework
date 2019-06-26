#pragma once

#include <string>

struct SDL_Window;
struct SDL_Surface;
struct SDL_Color;
struct _TTF_Font;

class Window
{
public:
	Window(int width, int height, const std::string& title);
	virtual ~Window();

	void stop();

protected:
	virtual int show(const std::string& title) = 0;
	void showText(const std::string& text, int x, int y, SDL_Surface* surface, _TTF_Font* font, const SDL_Color& color);

protected:
	// графические объекты
	SDL_Window* _window = nullptr;
	SDL_Surface* _screen = nullptr;
	_TTF_Font* _scoreFont = nullptr;
	_TTF_Font* _tipsFont = nullptr;

	// размеры окна
	int _scrWidth, _scrHeight;

	// флаг активности цикла обрабтки событий
	bool _active;
private:
};

