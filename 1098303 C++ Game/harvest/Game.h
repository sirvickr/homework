#pragma once

#include <ostream>
#include <string>
#include <list>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Color;
struct _TTF_Font;

class Cockroach;
class Fruit;

class Game
{
	struct StartParams {
		int size;
		int x;
		int y;
		int delta;
		const char* imgName;
	};
	using Beetles = std::list<Cockroach*>;
	typedef std::list<Fruit*> Fruits;

public:
	Game(int scrWidth, int scrHeight);
	~Game();
	int run();
	void stop();

private:
	/**
	* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
	* width and height
	* @param tex The source texture we want to draw
	* @param x The x coordinate to draw to
	* @param y The y coordinate to draw to
	* @param w The width of the texture to draw
	* @param h The height of the texture to draw
	*/
	void renderTexture(SDL_Texture *tex, int x, int y, int w, int h);

	/**
	* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
	* the texture's width and height
	* @param tex The source texture we want to draw
	* @param x The x coordinate to draw to
	* @param y The y coordinate to draw to
	*/
	void renderTexture(SDL_Texture *tex, int x, int y);

	/**
	* Render the message we want to display to a texture for drawing
	* @param message The message we want to display
	* @param fontFile The font we want to use to render the text
	* @param color The color we want the text to be
	* @param fontSize The size we want the font to be
	* @param renderer The renderer to load the texture in
	* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
	*/
	SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
		SDL_Color color, int fontSize);

	SDL_Texture* renderText(const std::string &message, _TTF_Font* font, SDL_Color color);

	/**
	* Log an SDL error with some error message to the output stream of our choice
	* @param os The output stream to write the message to
	* @param msg The error message to write, format will be msg error: SDL_GetError()
	*/
	void logSDLError(std::ostream &os, const std::string &msg);

	Beetles::iterator replaceCockroach(Beetles::iterator it, int index, int speed);
	Fruits::iterator newFruit(Fruits::iterator it);

	bool showScore();

private:
	//static StartParams startParams[];
	bool _active;
	Beetles beetles;
	Fruits fruits;

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	_TTF_Font* _font = nullptr;

	std::string userName;
};

