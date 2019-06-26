#include "Cockroach.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
// для генерации пседвослучайных чисел (ПСЧ)
#include <random>  
#include <ctime>

Cockroach::Cockroach(int fieldW, int fieldH, SDL_Surface* screen, const char* imageName, Orient orient, int d)
	: ScreenObject(screen), _orient(orient), _d(d), _fieldW(fieldW), _fieldH(fieldH)
{
	initGraphics(imageName);
}

bool Cockroach::initGraphics(const char* imageName) {
	if (!_screen || !imageName) {
		return false;
	}
	_surface = IMG_Load(imageName);
	if (_surface) {
		_pos.w = _surface->clip_rect.w;
		_pos.h = _surface->clip_rect.h;
		std::mt19937 gen;
		gen.seed(static_cast<uint32_t>(time(0)));
		switch (_orient) {
		case Orient::up:
			{
				std::uniform_int_distribution<> uidLoc(4, _fieldW - 5);
				_pos.x = uidLoc(gen);
				_pos.y = _fieldH;
				_d = -_d;

				_pos.x -= _pos.w / 2;
			}
			break;
		case Orient::down:
			{
				std::uniform_int_distribution<> uidLoc(4, _fieldW - 5);
				_pos.x = uidLoc(gen);
				_pos.y = -_pos.h;

				_pos.x -= _pos.w / 2;
			}
			break;
		case Orient::left:
			{
				std::uniform_int_distribution<> uidLoc(4, _fieldH - 5);
				_pos.x = _fieldW;
				_pos.y = uidLoc(gen);
				_d = -_d;

				_pos.y -= _pos.h / 2;
			}
			break;
		case Orient::right:
			{
				std::uniform_int_distribution<> uidLoc(4, _fieldH - 5);
				_pos.x = -_pos.w;
				_pos.y = uidLoc(gen);

				_pos.y -= _pos.h / 2;
				break;
			}
		}
		return true;
	}
	return false;
}

void Cockroach::move()
{
	int& pos = (Orient::up == _orient || Orient::down == _orient) ? _pos.y : _pos.x;
	pos += _d;
}

bool Cockroach::away() const
{
	switch (_orient) {
	case Orient::up:
		return (_pos.y < 0);
	case Orient::down:
		return (_pos.y > _fieldH);
	case Orient::left:
		return (_pos.x < 0);
	case Orient::right:
		return (_pos.x > _fieldW);
	}
	return false;
}

bool Cockroach::evade(int x, int y) const
{
	int x0 = _pos.x + 10;
	int y0 = _pos.y + 10;
	int x1 = _pos.x + _pos.w - 10;
	int y1 = _pos.y + _pos.h - 10;
	return !(x > x0 && y > y0 && x < x1 && y < y1);
}
