#pragma once

#include <ostream>
#include <string>
#include <list>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class Cockroach;

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

public:
	Game(int scrWidth, int scrHeight);
	~Game();
	int run();
	void stop();

private:
	/*
	* Loads an image into a texture on the rendering device
	* @param file The image file to load
	* @return the loaded texture, or nullptr if something went wrong.
	*/
	SDL_Texture* loadTexture(const std::string &file);

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
	* Log an SDL error with some error message to the output stream of our choice
	* @param os The output stream to write the message to
	* @param msg The error message to write, format will be msg error: SDL_GetError()
	*/
	void logSDLError(std::ostream &os, const std::string &msg);

	Beetles::iterator replaceCockroach(Beetles::iterator it, int index, int speed);

private:
	//static StartParams startParams[];
	bool _active;
	int _scrWidth, _scrHeight;
	Beetles beetles;

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

	std::string userName;
};

