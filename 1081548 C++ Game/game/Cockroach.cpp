#include "Cockroach.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

Cockroach::Cockroach()
{
}

Cockroach::Cockroach(SDL_Renderer* renderer, SDL_Texture* image, Orient orient, int x, int y, int d)
	: _orient(orient), _d(d), _x(x), _y(y)
{
	initGraphics(renderer, image);
}

Cockroach::~Cockroach()
{
}

void Cockroach::initGraphics(SDL_Renderer* renderer, SDL_Texture* image) {
	_renderer = renderer;
	_image = image;
	if(_image) {
		SDL_QueryTexture(_image, nullptr, nullptr, &_w, &_h);
	}
}

void Cockroach::move()
{
	int& dimention = (Orient::up == _orient || Orient::down == _orient) ? _y : _x;
	dimention += _d;
}

bool Cockroach::evade(int x, int y)
{
	int x0 = _x +10;
	int y0 = _y + 10;
	int x1 = _x + _w - 10;
	int y1 = _y + _h - 10;
	//std::cout << "evade(" << x << " " << y << "): " << " x0 " << x0 << " y0 " << y0 << " x1 " << x1 << " y1 " << y1 << " = " << std::boolalpha << (x > x0 && y > y0 && x < x1 && y < y1) << std::endl;
	return !(x > x0 && y > y0 && x < x1 && y < y1);
}

void Cockroach::draw()
{
	SDL_Rect dst;
	dst.x = _x;
	dst.y = _y;
	dst.w = _w;
	dst.h = _h;
	SDL_RenderCopy(_renderer, _image, nullptr, &dst);
}

#if 0
/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, nullptr, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
	int w, h;
	SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}
#endif
