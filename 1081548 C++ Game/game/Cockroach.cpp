#include "Cockroach.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
// ��� ��������� ��������������� ����� (���)
#include <random>  
#include <ctime>

Cockroach::Cockroach(int fieldW, int fieldH, SDL_Renderer* renderer, const char* imageName, Orient orient, /*int x, int y,*/ int d)
	: _orient(orient), _d(d), /*_x(x), _y(y),*/ _fieldW(fieldW), _fieldH(fieldH)
{
	initGraphics(renderer, imageName);
}

Cockroach::~Cockroach()
{
	if (_image) {
		SDL_DestroyTexture(_image);
		_image = nullptr;
	}
}

bool Cockroach::initGraphics(SDL_Renderer* renderer, const char* imageName) {
	if (!renderer || !imageName) {
		return false;
	}
	_renderer = renderer;
	_image = IMG_LoadTexture(renderer, imageName);
	if (_image && 0 == SDL_QueryTexture(_image, nullptr, nullptr, &_w, &_h)) {
		std::mt19937 gen;
		gen.seed(static_cast<uint32_t>(time(0)));
		//std::uniform_int_distribution<> uidSpeed(1, 3);
		switch (_orient) {
		case Orient::up:
			{
				std::uniform_int_distribution<> uidLoc(4, _fieldW - 5);
				_x = uidLoc(gen);
				_y = _fieldH; //  startPoint.y;
				_d = -_d;
				//cr = new Cockroach(_renderer, startPoint.imgName, orient,
				//	x, y, startPoint.delta * uidSpeed(gen));

				_x -= _w / 2;
			}
			break;
		case Orient::down:
			{
				std::uniform_int_distribution<> uidLoc(4, _fieldW - 5);
				_x = uidLoc(gen);
				_y = -_h; //  startPoint.y;
				//cr = new Cockroach(_renderer, startPoint.imgName, orient,
				//	x, y, startPoint.delta * uidSpeed(gen));

				_x -= _w / 2;
			}
			break;
		case Orient::left:
			{
				std::uniform_int_distribution<> uidLoc(4, _fieldH - 5);
				_x = _fieldW; // startPoint.x;
				_y = uidLoc(gen);
				_d = -_d;
				//cr = new Cockroach(_renderer, startPoint.imgName, orient,
				//	x, y, startPoint.delta * uidSpeed(gen));

				_y -= _h / 2;
			}
			break;
		case Orient::right:
			{
				std::uniform_int_distribution<> uidLoc(4, _fieldH - 5);
				_x = -_w; //  startPoint.x;
				_y = uidLoc(gen);
				//cr = new Cockroach(_renderer, startPoint.imgName, orient,
				//	x, y, startPoint.delta * uidSpeed(gen));

				_y -= _h / 2;
				break;
			}
		}
		return true;
	}
	return false;
}

void Cockroach::move()
{
#if 1
	int& pos = (Orient::up == _orient || Orient::down == _orient) ? _y : _x;
	pos += _d;
#else
	switch (_orient) {
	case Orient::up:
		_y += _d;
		if (_y < 0) {
			_y = _fieldH;
		}
		break;
	case Orient::down:
		_y += _d;
		if (_y > _fieldH) {
			_y = -_h;
		}
		break;
	case Orient::left:
		_x += _d;
		if (_x < 0) {
			_x = _fieldW;
		}
		break;
	case Orient::right:
		_x += _d;
		if (_x > _fieldW) {
			_x = _w;
		}
		break;
	}
#endif
}

bool Cockroach::away() const
{
	switch (_orient) {
	case Orient::up:
		return (_y < 0);
	case Orient::down:
		return (_y > _fieldH);
	case Orient::left:
		return (_x < 0);
	case Orient::right:
		return (_x > _fieldW);
	}
	return false;
}

bool Cockroach::evade(int x, int y) const
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
	//std::cout << "\ndraw: R " << _renderer << " I " << _renderer << ": " << dst.x << " " << dst.y << "      " << " w " << dst.w << " h " << dst.h << std::endl;
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
